// gcc -Werror -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=undefined -fno-diagnostics-color -g3 -o re re.c
#include <stddef.h>
#include <stdint.h>

typedef uint8_t    u8;
typedef int32_t    b32;
typedef int32_t    i32;
typedef uint32_t   u32;
typedef int64_t    i64;
typedef uint64_t   u64;
typedef char       byte;
typedef ptrdiff_t  size;
typedef size_t     usize;
typedef intptr_t   iptr;

struct s8;

#define assert(c)     while (!(c)) *(volatile int *)0 = 0
#define sizeof(x)     (size) sizeof(x)
#define countof(a)    (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s)   (countof(s) - 1)
#define s8(s)         (struct s8){(u8 *)s, lengthof(s)}
#define s8cstr(s)     (struct s8){(u8 *)s, strlen(s)}
#define s8nul         (struct s8){(u8 *)"", 1}
#define xset(d, c, n) __builtin_memset(d, c, n)
#define xcpy(d, s, n) __builtin_memcpy(d, s, n)

static usize to_usize(size v)
{
    assert(v >= 0);
    return (usize) v;
}

enum {
    PLT_READ  = 0x1,
    PLT_WRITE = 0x2,
    PLT_EXEC  = 0x4
};

enum {
    PLT_MAP_SHARED  = 0x01,
    PLT_MAP_PRIVATE = 0x02,
    PLT_MAP_ANON    = 0x20
};

#define PLT_MAP_FAILED ((void *) -1)

static u8 *plt_mmap(size, i32, i32);
static b32 plt_write(i32, u8 *, size);
static void plt_exit(i32 rc);

struct arena {
    u8 *beg;
    u8 *end;
    size sz;
};

static void oom(void)
{
    plt_exit(1);
}
    
#define new(a, t, n) (t *) alloc(a, sizeof(t), _Alignof(t), n)
static void *alloc(struct arena *a, size sz, size align, size count)
{
    size pad = -(iptr) a->beg & (align - 1);
    size avail = a->end - a->beg - pad;
    if (avail < 0 || count > avail / sz) {
        oom();
    }
    
    u8 *p = a->beg + pad;
    a->beg += pad + count * sz;

    return xset(p, 0, (usize) (count * sz));
}

struct buf
{
    u8 *buf;
    size cap;
    size len;
    int fd;
    int err;
};

static struct buf out = { (u8[1 << 8]) { 0 }, 1 << 8, 0, 1, 0 };
static struct buf err = { (u8[1 << 8]) { 0 }, 1 << 8, 0, 2, 0 };

static void flush(struct buf *b)
{
    b->err |= b->fd < 0;
    if (!b->err && b->len) {
        b->err |= plt_write(b->fd, b->buf, b->len) < b->len;
        b->len = 0;
    }
}

static void append(struct buf *b, u8 *src, size len)
{
    u8 *end = src + len;
    while (!b->err && src < end) {
        size left = end - src;
        size avail = b->cap - b->len;
        size amt = avail < left ? avail : left;

        for (size i = 0; i < amt; i++) {
            b->buf[b->len + i] = src[i];
        }
        b->len += amt;
        src += amt;

        if (amt < left) {
            flush(b);
        }
    }
}

struct s8
{
    u8 *data;
    size len;
};

static struct s8 s8cpy(struct arena *a, struct s8 s)
{
    struct s8 r = s;
    r.data = new(a, u8, s.len);
    if (r.len)
    {
        xcpy(r.data, s.data, (usize) r.len);
    }
    return r;
}

static struct s8 s8cat(struct arena *a, struct s8 head, struct s8 tail)
{
    if (!head.data || head.data + head.len != a->beg)
    {
        head = s8cpy(a, head);
    }
    head.len += s8cpy(a, tail).len;
    return head;
}

static i32 re_(i32 argc, u8 **argv, struct arena *a)
{
    if (argc < 3) {
        
    }
    return 0;
}

static i32 re(i32 argc, u8 **argv, u8 *mem, size cap)
{
    struct arena a = { mem, mem + cap, cap };

    return re_(argc, argv, &a);
}

#if defined(__linux__)
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static u8 *plt_mmap(size sz, i32 prot, i32 flgs)
{
    return (u8 *) mmap(0, to_usize(sz), prot, flgs, -1, 0);
}

static b32 plt_write(i32 fd, u8 *buf, size len)
{
    return len == write(fd, buf, to_usize(len));
}

static void plt_exit(i32 rc)
{
    _exit(rc);
}

int main(int argc, char **argv)
{
    size cap = (size) 1 << 24;
    u8 *mem = mmap(0, to_usize(cap), PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    i32 rc = re(argc, (u8 **) argv, mem, cap);
    _exit(EXIT_SUCCESS);
}

#else
#error "Unsupported platform, consider porting"
#endif
