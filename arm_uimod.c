// gcc -Werror -std=gnu99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fno-builtin -g3 -o arm_uimod arm_uimod.c
#include <stdlib.h>
#include <stdio.h>

__attribute__((naked))
static unsigned int ret_uidivmod(unsigned int q, unsigned int r)
{
    (void) q;
    (void) r;
    __asm(
        "bx lr"
        );
}

static void uidivmod(unsigned int n, unsigned int d, unsigned int *q, unsigned int *r)
{
    *q = 0;
    *r = 0;
    if (d == 0)
    {
        return;
    }
    if (d <= n)
    {
        while (n >= d)
        {
            n -= d;
            (*q)++;
        }
    }
    *r = n;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static unsigned int __aeabi_uidivmod(unsigned int n, unsigned int d)
{
    unsigned int q;
    unsigned int r;
    
    uidivmod(n, d, &q, &r);
    return ret_uidivmod(q, r);

}
#pragma GCC diagnostic pop

int main(int argc, char **argv)
{
    (void) argc;
    unsigned int i = (unsigned int) atoi(argv[1]);
    unsigned int j = (unsigned int) atoi(argv[2]);
    printf("%d\n", i % j);
    return EXIT_SUCCESS;
}
