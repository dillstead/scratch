// gcc -g3 -Werror -Wall -Wextra -fno-builtin -std=gnu99 -nostdlib -O2 -o arm_aux arm_aux.c
#include <elf.h>

int start(int argc, char **argv, char **env, Elf32_auxv_t *aux)
{
    (void) argc;
    (void) argv;
    (void) env;
    (void) aux;
    
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
        "bl      start\n"
        "mov     r7, #1\n"
        "swi     #0\n"
        );
}
