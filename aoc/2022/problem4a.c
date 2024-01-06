// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem4a problem4a.c
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int a, b, c, d;
    int num = 0;
    
    while (scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4)
    {
        if ((a <= c && b >= d) || (a >= c && b <= d))
        {
            num++;
        }
    }
    printf("%d\n", num);
    return EXIT_SUCCESS;
}
