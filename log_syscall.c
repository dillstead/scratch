#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <errno.h>

long my_syscall1(long num, ...)
{
    printf("%s enter: id %ld\n", __func__, num);
    va_list ap;
    va_start(ap, num);
    long ret = syscall(num, ap);
    va_end(ap);
    printf("%s exit: ret %d\n", __func__, ret);
    return ret;
}


__attribute__((naked))
long my_syscall2(long num, ...)
{
    (void) num;
    __asm volatile(
        "push {r0-r4, lr}\n"
        "mov r1, r0\n"
        "adr r0, 1f\n"
        "blx printf\n"
        "ldmia sp, {r0-r4}\n"
        "blx syscall\n"
        "str r0, [sp]\n"
        "mov r1, r0\n"
        "adr r0, 2f\n"
        "blx printf\n"
        "pop {r0-r4, pc}\n"
        "1: .asciz \"syscall enter: id %ld\\n\"\n"
        "2:  .asciz \"syscall exit: ret %ld\\n\"\n"
        );
}

int main(void)
{
    my_syscall1(SYS_write, 1, "Hello World!\n", sizeof "Hello World!\n" - 1);
    my_syscall2(SYS_write, 1, "Hello World!\n", sizeof "Hello World!\n" - 1);
    return EXIT_SUCCESS;
}
