// gcc -g3 -Werror -Wall -Wextra -fno-builtin -std=gnu99 -nostdlib -O2 -o arm_startup_dump arm_startup_dump.c
#include <elf.h>
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

#define sizeof(x)   (size) sizeof(x)
#define countof(a)  (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#define SYSCALL3(n, a, b, c) \
    syscall3(n,(long)(a),(long)(b),(long)(c))

static i32 slen(const char *str)
{
    if (!str)
    {
        return 0;
    }
    
    i32 len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
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
        : "r9", "r12", "r14", "memory"
    );
    return ret;
}

#define duitoa(i,s) uitoa((i),(s),10,"0123456789")
#define xuitoa(i,s) uitoa((i),(s),16,"0123456789abcdef")

static char *uitoa(unsigned int i, char *s, int base, const char *digits)
{
    int len = 1;
    int t = i;
    while (i /= base) len++;
    char *beg = s + len;
    *beg = '\0';
    do
    {
        *--beg = digits[t % base];
    } while (t /= base);
    return s;
}

int start(int argc, char **argv, char **env, Elf32_auxv_t *aux, void *ii)
{
    (void) ii;

    SYSCALL3(SYS_write, 1, "Arguments:\n", lengthof("Arguments:\n"));
    for (int i = 0; i < argc; i++)
    {
        SYSCALL3(SYS_write, 1, argv[i], slen(argv[i]));
        SYSCALL3(SYS_write, 1, "\n", lengthof("\n"));
    }
    SYSCALL3(SYS_write, 1, "Environment:\n", lengthof("Environment:\n"));
    for (char **e = env; *e; e++)
    {
        SYSCALL3(SYS_write, 1, *e, slen(*e));
        SYSCALL3(SYS_write, 1, "\n", lengthof("\n"));
    }
    SYSCALL3(SYS_write, 1, "Auxiliary Vector:\n", lengthof("Auxiliary Vector:\n"));
    for (Elf32_auxv_t *a = aux; a->a_type != AT_NULL; a++)
    {
        char val[64];
        duitoa(a->a_type, val);
        SYSCALL3(SYS_write, 1, val, slen(val));
        SYSCALL3(SYS_write, 1, ":", lengthof(":"));
        duitoa(a->a_un.a_val, val);
        SYSCALL3(SYS_write, 1, val, slen(val));
        SYSCALL3(SYS_write, 1, "(0x", lengthof("(0x"));
        xuitoa(a->a_un.a_val, val);
        SYSCALL3(SYS_write, 1, val, slen(val));
        SYSCALL3(SYS_write, 1, ")\n", lengthof(")\n"));
    }
    return 1;
}

__attribute__((naked, noreturn))
void _start(void)
{
        __asm volatile (
        "ldr     r0, [sp]\n"
        "add     r1, sp, #4\n"
        "add     r2, r1, r0, lsl #2\n"
        "mov     r3, r2\n"
        "add     r2, r2, #4\n"
        "not_zero:\n"
        "add     r3, r3, #4\n"
        "ldr     r4, [r3]\n"
        "cmp     r4, #0\n"
        "bne     not_zero\n"
        "add     r3, r3, #4\n"
        "sub     sp, sp, #4\n"
        "eor     r5, r5\n"
        "str     r5, [sp]\n"
        "bl      start\n"
        "mov     r7, #1\n"
        "swi     #0\n"
        );
}
