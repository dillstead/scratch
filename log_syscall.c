#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
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
        // Preserve callee-saved registers making sure
        // to push an even number of registers to keep the
        // SP 8-byte aligned.
        "push {r4-r8, lr}\n"  
        // Save parameter registers to callee-saved
        // registers so they are preserved across the 
        // call to printf.
        "mov r5, r0\n"
        "mov r6, r1\n"
        "mov r7, r2\n"
        "mov r8, r3\n"
        // Setup arguments and call printf.
        "mov r1, r0\n"
        "adr r0, 1f\n"
        "blx printf\n" // Ignore return
        // Move possible stack arguments to a "shadow" frame 
        // so they are in their correct position when syscall 
        // is called.
        // NOTE: 3 stack arguments supported
        // Maximum number of syscall arguments is 7, 
        // 4 register arguments + 3 stack arguments.
        //
        // Make sure the SP remains 8 byte aligned as only 3 
        // registers are "pushed".
        "ldr r0, [sp, #24]\n"
        "ldr r1, [sp, #28]\n"
        "ldr r2, [sp, #32]\n"
        "sub sp, sp, #16\n" 
        "str r0, [sp, #0]\n"
        "str r1, [sp, #4]\n"
        "str r2, [sp, #8]\n"
        // Restore original argument registers before
        // calling syscall.
        "mov r0, r5\n"
        "mov r1, r6\n"
        "mov r2, r7\n"
        "mov r3, r8\n"
        "blx syscall\n"  // Return value in r0
        // Discard the shadow frame.
        "add sp, sp, #16\n"
        // Save syscall return and errno.
        "mov r4, r0\n"
        "blx __errno_location\n"
        "ldr r5, [r0]\n"
        // Setup arguments to printf.
        "mov r1, r0\n"
        "adr r0, 2f\n"
        "blx printf\n" // Ignore return
        // Restore syscall return and errno.
        "blx __errno_location\n"
        "str r5, [r0]\n"
        "mov r0, r4\n"
        // Restore callee-saved registers and return to the caller.
        "pop {r4-r8, pc}\n"
        "1: .asciz \"syscall enter: id %ld\\n\"\n"
        "2: .asciz \"syscall exit: ret %ld\\n\"\n"
        );
}

int main(void)
{
    my_syscall1(SYS_write, 1, "Hello World!\n", sizeof "Hello World!\n" - 1);
    my_syscall2(SYS_write, 1, "Hello World!\n", sizeof "Hello World!\n" - 1);
    my_syscall1(SYS_mmap2, NULL, 1 << 12, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    my_syscall2(SYS_mmap2, NULL, 1 << 12, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return EXIT_SUCCESS;
}
