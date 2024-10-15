#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

// gcc -std=gnu11 -O2 -o coro coro.c

// Coroutines from a bit of assembly
//
// The interface is a struct (coro) and a function (yield). Coroutines
// pass values between each other through _yield. The first value is
// passed as the argument to the coroutine function, and the rest are
// returned from yield(). Coroutine functions must not return.
// 
// This is free and unencumbered software released into the public domain.

// ARM port of coroutine implementation:
//  https://github.com/skeeto/scratch/blob/master/misc/coro.c

typedef struct {
    void **rsp;
} coro;

extern void *_yield(coro *c, void *arg);

__attribute((naked))
static void *yield(coro *c, void *arg)
{
    (void) c;
    (void) arg;
    asm (
        "push    { r4-r11, lr }\n" // save context
        "ldr     r2, [r0]      \n" // switch stacks
        "str     sp, [r0]      \n"
        "mov     sp, r2        \n"
        "pop     { r4-r11, lr }\n" // restore context
        "mov     r0, r1        \n" // pass arg as first arg and return value
        "mov     pc, lr        \n" // switch to other coroutine
    );
}

#define assert(c)     while (!(c)) *(volatile int *)0 = 0
#define countof(a)    (size)(sizeof(a) / sizeof(*(a)))
#define new(a, t, n)  (t *)alloc(a, sizeof(t), _Alignof(t), n)

typedef unsigned char      u8;
typedef   signed int       b32;
typedef   signed int       i32;
typedef   signed long long i64;
typedef unsigned long long u64;
typedef   signed long      size;
typedef   signed long      iptr;
typedef unsigned long      uptr;

typedef struct {
    char *beg;
    char *end;
} arena;

static void *alloc(arena *a, size objsize, size align, size count)
{
    size padding = (uptr)a->end & (align - 1);
    assert(count <= (a->end - a->beg - padding)/objsize);
    size total = objsize * count;
    char *r = a->end -= total + padding;
    for (size i = 0; i < total; i++) {
        r[i] = 0;
    }
    return r;
}

static coro *newcoro(arena *perm, void corofunc(void *))
{
    coro *c = new(perm, coro, 1);
    size cap = 1<<10;
    c->rsp = alloc(perm, 8, 8, cap/8);
    // Seed the stack with corofunc as lr
    *(c->rsp + cap/4 - 1) = (void *)corofunc;
    c->rsp += cap/4 - 9;
    return c;
}

typedef struct {
    i32 fd;
    u8  *buf;
    i32  len;
} message;

// Print each message passed to the coroutine. Pass the return coroutine
// for the first call.
static void print(void *arg)
{
    coro *ret = arg;
    for (;;) {
        message *m = yield(ret, 0);
        write(m->fd, m->buf, m->len);
    }
}

// Test the print coroutine function by printing two strings.
static void simpleprint(arena scratch)
{
    coro *c = newcoro(&scratch, print);
    yield(c, c);  // prime the printer
    static u8 hello[] = "Hello, ";
    static u8 world[] = "world!\n";
    yield(c, &(message){1, hello, countof(hello)-1});
    yield(c, &(message){1, world, countof(world)-1});
}

typedef struct {
    i64   lo;
    i64   hi;
    coro *ret;
} rangectx;

// Generate 64-bit integers in [lo, hi). Pass a rangectx in the first
// call to configure it. Each yield returns a i64 pointer to the next
// value. Returns null on completion.
static void range(void *arg)
{
    rangectx *ctx = arg;
    for (i64 i = ctx->lo; i < ctx->hi; i++) {
        yield(ctx->ret, &i);
    }
    yield(ctx->ret, 0);
}

// Test the range generator by printing some numbers. Uses two
// coroutines simultaneously.
static void rangetest(arena scratch)
{
    message msg = {0};
    msg.fd = 1;
    coro *prtcoro = newcoro(&scratch, print);
    yield(prtcoro, prtcoro);  // prime the printer

    coro *rngcoro = newcoro(&scratch, range);
    rangectx rng = {1000, 1015, rngcoro};
    for (i64 *r = yield(rngcoro, &rng); r; r = yield(rngcoro, 0)) {
        u8  buf[32];
        u8 *end = buf + countof(buf);
        u8 *beg = end;
        *--beg = '\n';
        i64 x = *r;
        do {
            *--beg = (u8)(x%10) + '0';
        } while (x /= 10);
        *--beg = '=';
        *--beg = 'i';
        msg.buf = beg;
        msg.len = (i32)(end - beg);
        yield(prtcoro, &msg);
    }
}

static void printnum(i32 h, i64 x, i32 i, i32 width)
{
    u8  buf[32];
    u8 *end = buf + countof(buf);
    u8 *beg = end;
    i32 cols = 80 / width;
    if (i%cols == cols-1) {
        *--beg = '\n';
    }
    do {
        *--beg = (u8)(x%10) + '0';
    } while (x /= 10);
    while (end-beg < width) {
        *--beg = ' ';
    }
    write(h, beg, (i32)(end-beg));
}

// Initializes a coroutine prime number filter.
typedef struct {
    i64   prime;
    coro *next;  // next filter
    coro *prev;  // calling coroutine
} filterctx;

typedef struct {
    i64   candidate;
    coro *prev;  // calling coroutine
} query;

// Predicate filter for one prime number. yield once with a filterctx to
// configure it, then yield with query objects.
static void filter(void *arg)
{
    filterctx ctx = *(filterctx *)arg;
    for (b32 result = 0;;) {
        query *q = yield(ctx.prev, &result);
        if (q->candidate % ctx.prime == 0) {
            result = 0;
        } else if (!ctx.next) {
            result = 1;
        } else {
            query next = {0};
            next.candidate = q->candidate;
            next.prev = ctx.next;
            result = *(b32 *)yield(ctx.next, &next);
        }
    }
}

// Sieve of Eratosthenes using coroutines.
static void primes(arena scratch, i32 limit)
{
    i32 count = 0;
    i32 stdout = 1;

    // Initialize filter with 2
    query q = {0};
    q.prev = newcoro(&scratch, filter);
    yield(q.prev, &(filterctx){2, 0, q.prev});
    printnum(stdout, 2, count++, 6);

    for (q.candidate = 3; count < limit; q.candidate++) {
        b32 r = *(b32 *)yield(q.prev, &q);
        if (r) {
            coro *c = newcoro(&scratch, filter);
            yield(c, &(filterctx){q.candidate, q.prev, c});
            q.prev = c;
            printnum(stdout, q.candidate, count++, 6);
        }
    }
}

int main(void)
{
    size cap = 1<<24;
    arena scratch = {0};
    scratch.beg = mmap(NULL, cap, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    scratch.end = scratch.beg + cap;

    simpleprint(scratch);
    rangetest(scratch);
    primes(scratch, 13*50);
    return EXIT_SUCCESS;
}
