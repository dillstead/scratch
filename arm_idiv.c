// gcc -Werror -std=gnu99 -Wall -Wextra -fno-builtin -nostdlib -O2 -o arm_idiv arm_idiv.c
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>

#define SYSCALL1(n, a)       \
    syscall1(n,(long)(a))
#define SYSCALL3(n, a, b, c) \
    syscall3(n,(long)(a),(long)(b),(long)(c))

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
int __aeabi_idiv(int n, int d)
{
    if (d == 0)
    {
        return 0;
    }
    int q = 0;
    int s = 1;
    if (n < 0)
    {
        s *= -1;
        n = -n;
    }
    if (d < 0)
    {
        s *= -1;
        d = -d;
    }
    while (n >= d)
    {
        n -= d;
        q++;
    }
    return q * s;
}
#pragma GCC diagnostic pop

static int xatoi(const char *s)
{
    const char *beg = s;
    int sn = 1;
    if (*beg == '-')
    {
        sn = -1;
        beg++;
    }
    
    int res = 0;
    while (*beg)
    {
        int dig = *beg - '0';
        if (sn == 1)
        {
            if (res > INT_MAX / 10
                || (res == INT_MAX / 10 && dig > INT_MAX % 10))
            {
                return INT_MAX;
            }
            res *= 10;
            res += dig;
        }
        else
        {
            if (res < INT_MIN / 10
                || (res == INT_MIN / 10 && -dig < INT_MIN % 10))
            {
                return INT_MIN;
            }
            res *= 10;
            res -= dig;
        }
        beg++;
    }
    return res;
}

static char *xitoa(int i, char *s)
{
    int len = 1;
    int t = i > 0 ? -i : i;
    if (i < 0)
    {
        *s = '-';
        len++;
    }
    while (i /= 10) len++;
    char *beg = s + len;
    *beg = '\0';
    do
    {
        *--beg = '0' - t % 10;
    } while (t /= 10);
    return s;
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

static void start(int argc, char **argv)
{
    (void) argc;
    int i = xatoi(argv[1]);
    int j = xatoi(argv[2]);
    char buf[24];
    
    xitoa(i / j, buf);
    SYSCALL3(SYS_write, 1, buf, slen(buf));
    SYSCALL3(SYS_write, 1, "\n", 1);
    SYSCALL1(SYS_exit, 0);    
}

__attribute__((naked, noreturn))
void _start(void)
{
    register int argc asm("r0");
    register char **argv asm("r1");
    
    __asm volatile (
        "ldr     r0, [sp]\n"
        "add     r1, sp, #4\n"
        :
        :
        :"r0", "r1");
    start(argc, argv);
}
