// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem4b problem4b.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int a, b, c, d;
    int num = 0;
    
    while (scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4)
    {
        if ((a >= c && a <= d) || (c >= a && c <= b))
        {
            num++;
        }
    }
    printf("%d\n", num);
    return EXIT_SUCCESS;
}
