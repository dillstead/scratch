#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define SYSCALL3(n, a, b, c)          \
    syscall3(n,(long)(a),(long)(b),(long)(c))

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

int main(void)
{
    printf("%d\n", SYSCALL3(SYS_open, "foo", O_RDONLY, 0666));
    printf("%d\n", open("foo", O_RDONLY, 0666));
    printf("%d\n", errno);
}

