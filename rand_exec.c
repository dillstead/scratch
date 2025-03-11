// gcc -Werror -std=gnu99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fno-builtin -nostdlib -g3 -o rand_exec rand_exec.c -lgcc
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <elf.h>
#include <sys/mman.h>
#include <syscall.h>

typedef __UINT8_TYPE__   u8;
typedef __INT8_TYPE__    i8;
typedef __UINT16_TYPE__  u16;
typedef __INT16_TYPE__   i16;
typedef __UINT32_TYPE__  u32;
typedef __INT32_TYPE__   i32;
typedef __UINT64_TYPE__  u64;
typedef __INT64_TYPE__   i64;
typedef __PTRDIFF_TYPE__ size;
typedef __SIZE_TYPE__    usize;
typedef __UINTPTR_TYPE__ uptr;
typedef __INTPTR_TYPE__  iptr;
typedef char             byte;
typedef unsigned char    bool;
enum
{
   false,
   true
};

#define PGSIZE  (1 << 12)

#define sizeof(x)   (size) sizeof(x)
#define countof(a)  (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)
#define s8(s)       (struct s8){(u8 *)s, lengthof(s)}
#define s8nul       (struct s8){(u8 *)"", 1}

#define SYSCALL1(n, a)                \
    syscall1(n,(long)(a))
#define SYSCALL2(n, a, b)             \
    syscall2(n,(long)(a),(long)(b))
#define SYSCALL3(n, a, b, c)          \
    syscall3(n,(long)(a),(long)(b),(long)(c))
#define SYSCALL6(n, a, b, c, d, e, f) \
    syscall6(n,(long)(a),(long)(b), (long)(c), (long)(d), (long)(e), (long)(f))

static long syscall1(long n, long a)
{
    register long ret asm("r0");
    register long r7 asm("r7") = n;
    register long r0 asm("r0") = a;
    __asm volatile (
        "swi #0\n"
        : "=r"(ret)
        : "r"(r7), "r"(r0)
        : "r1", "r2", "r3", "r4", "r9", "r12", "memory"
    );
    return ret;
}

static long syscall2(long n, long a, long b)
{
    register long ret asm("r0");
    register long r7 asm("r7") = n;
    register long r0 asm("r0") = a;
    register long r1 asm("r1") = b;
    __asm volatile (
        "swi #0\n"
        : "=r"(ret)
        : "r"(r7), "r"(r0), "r"(r1)
        : "r2", "r3", "r4", "r9", "r12", "memory"
    );
    return ret;
}

static long syscall3(long n, long a, long b, long c)
{
    register long ret asm("r0");
    register long r7 asm("r7") = n;
    register long r0 asm("r0") = a;
    register long r1 asm("r1") = b;
    register long r2 asm("r2") = c;
    __asm volatile (
        "swi #0\n"
        : "=r"(ret)
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2)
        : "r3", "r4", "r9", "r12", "memory"
    );
    return ret;
}

static long syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long ret asm("r0");
    register long r7 asm("r7") = n;
    register long r0 asm("r0") = a;
    register long r1 asm("r1") = b;
    register long r2 asm("r2") = c;
    register long r3 asm("r3") = d;
    register long r4 asm("r4") = e;
    register long r5 asm("r5") = f;
    __asm volatile (
        "swi #0\n"
        : "=r"(ret)
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
        : "r9", "r12", "memory"
    );
    return ret;
}

void raise(int sig)
{
    (void) sig;
    SYSCALL1(SYS_exit, 1);
}

static void *mset(void *s, u8 c, size n)
{
    u8 *p = s;
    while (n--) *p++ = c;
    return s;
}

static void *mcpy(void *d, void *s, size n)
{
    u8 *p = d;
    u8 *q = s;
    while (n--) *p++ = *q++;
    return d;
}

typedef void *jmp_buf[10];

__attribute__((naked, returns_twice))
int xsetjmp(jmp_buf buf)
{
    (void) buf;
    __asm(
        "stm r0, { r4-r11, sp, lr }\n"
        "eor r0, r0\n"
        "mov pc, lr\n"
        );
}

__attribute__((naked, noreturn))
void xlongjmp(jmp_buf buf, int ret)
{
    (void)buf;
    (void)ret;
    __asm(
        "ldm r0, { r4-r11, sp, lr }\n"
        "mov r0, r1\n"
        "mov pc, lr\n"
        );
}

struct arena
{
    u8 *beg;
    u8 *end;
    size sz;
    u8 **commit;
    jmp_buf *ctx;
};

static bool new_arena(struct arena *a, jmp_buf *ctx, size sz)
{
    if (sz <= 0)
    {
        return false;
    }
    a->sz = sz + sizeof(a->commit);
    a->beg = (u8 *) SYSCALL6(SYS_mmap2, NULL, a->sz, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                                 -1, 0);
    if (a->beg == MAP_FAILED)
    {
        return false;
    }
    if (SYSCALL3(SYS_mprotect, a->beg, PGSIZE, PROT_READ | PROT_WRITE) == -1)
    {
        return false;
    }
    a->end = a->beg + a->sz;
    a->commit = (u8 **) a->beg;
    *a->commit = a->beg + PGSIZE;
    a->beg += sizeof(a->commit);
    a->ctx = ctx;
    return true;
}

static void free_arena(struct arena *a)
{
    if (!a->beg)
    {
        SYSCALL2(SYS_munmap, a->beg, a->sz);
    }
}

#define new(a, t, n) (t *) alloc(a, sizeof(t), _Alignof(t), n)
static void *alloc(struct arena *a, size sz, size align, size count)
{
    size pad = -(iptr) a->beg & (align - 1);
    size avail = a->end - a->beg - pad;
    if (avail < 0 || count > avail / sz)
    {
        xlongjmp(*a->ctx, 1);
    }
    u8 *p = a->beg + pad;
    a->beg += pad + count * sz;
    if (a->beg >= *a->commit)
    {
        if (SYSCALL3(SYS_mprotect, *a->commit, (size_t) (a->beg - *a->commit + 1),
                     PROT_READ | PROT_WRITE) == -1)
        {
            xlongjmp(*a->ctx, 1);
        }
        *a->commit = (u8 *) ((iptr) (a->beg + (PGSIZE - 1)) & ~(PGSIZE - 1));
    }
    return mset(p, 0, count * sz);
}

static long slen(const char *str)
{
    if (!str)
    {
        return 0;
    }
    long len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
}

struct s8
{
    u8  *data;
    size len;
};

static struct s8 s8cpy(struct arena *a, struct s8 s)
{
    struct s8 r = s;
    r.data = new(a, u8, s.len);
    if (r.len)
    {
        mcpy(r.data, s.data, r.len);
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

struct linux_dirent64
{
    u64	d_ino;
    i64	d_off;
    u16 d_reclen;
    unsigned char d_type;
    char d_name[];
};

static bool is_exec(struct arena scratch, struct s8 fname)
{
    int fd = -1;
    Elf32_Ehdr ehdr;
    bool exec = false;

    fname = s8cat(&scratch, fname, s8nul);
    if ((fd = (int) SYSCALL2(SYS_open, fname.data, O_RDONLY)) < 0
        || SYSCALL3(SYS_read, fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
    {
        goto cleanup;
    }

    exec = ehdr.e_ident[0] == ELFMAG0
        && ehdr.e_ident[1] == ELFMAG1
        && ehdr.e_ident[2] == ELFMAG2
        && ehdr.e_ident[3] == ELFMAG3
        && ehdr.e_type == ET_EXEC
        && ehdr.e_machine == EM_ARM
        && ehdr.e_version == EV_CURRENT;

cleanup:
    SYSCALL1(SYS_close, fd);
    return exec;
}

static unsigned int prand_next(unsigned int *state)
{
    *state = (1664525 * (*state) + 1013904223);
    return *state;
}

static bool pick_exec(struct arena *perm, struct arena scratch, struct s8 *ename)
{
    struct arena ptmp = *perm;
    bool picked = false;

    int dfd = -1;
    if ((dfd = (int) SYSCALL2(SYS_open, ".", O_RDONLY)) < 0)
    {
        goto cleanup;
    }

    struct timespec ts;
    if (SYSCALL2(SYS_clock_gettime, CLOCK_REALTIME, &ts) < 0)
    {
        goto cleanup;
    }
    unsigned int state = (unsigned int) (ts.tv_nsec * 1000000000 + ts.tv_sec);
    
    char dents[1024];
    long nbytes;
    unsigned int cnt = 1;
    while ((nbytes = SYSCALL3(SYS_getdents64, dfd, dents, sizeof(dents))) > 0)
    {
        long pos = 0;
        while (pos < nbytes)
        {
            struct arena stmp = scratch;
            struct linux_dirent64 *dent = (struct linux_dirent64 *) (dents + pos);
            if (dent->d_type == DT_REG)
            {
                struct s8 fname = { (u8 *) dent->d_name, slen(dent->d_name) };
                fname = s8cat(&stmp, s8("./"), fname);
                if (is_exec(stmp, fname))
                {
                    if (prand_next(&state) % cnt == 0)
                    {
                        picked = true;
                        ptmp = *perm;
                        *ename = s8cpy(&ptmp, fname);
                    }
                    cnt++;
                }
            }
            pos += dent->d_reclen;
        }
    }
    *perm = ptmp;        
    
cleanup:
    SYSCALL1(SYS_close, dfd);
    return picked;
}

__attribute__((used))
static void start(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    volatile int rc = 1;
    jmp_buf ctx;
    struct arena perm = { 0 };
    struct arena scratch = { 0 };
    if (!new_arena(&perm, &ctx, 1 << 12)
        || !new_arena(&scratch, &ctx, 1 << 12))
    {
        goto cleanup;
    }

    if (xsetjmp(ctx))
    {
        // OOM
        goto cleanup;
    }

    struct s8 fname;
    if (!pick_exec(&perm, scratch, &fname))
    {
        goto cleanup;
    }
    if (SYSCALL3(SYS_write, 1, fname.data, fname.len) < fname.len
        || SYSCALL3(SYS_write, 1, "\n", 1) < 1)
    {
        goto cleanup;
    }
    rc = 0;

cleanup:
    free_arena(&scratch);
    free_arena(&perm);
    SYSCALL1(SYS_exit, rc);
}

__attribute__((naked, noreturn))
void _start(void)
{
    __asm(
        "ldr r0, [sp]\n"
        "add r1, sp, #4\n"
        "b   start\n"
        );
}

