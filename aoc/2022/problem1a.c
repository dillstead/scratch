// gcc -Werror -funsigned-char -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem1a problem1a.c
#include <stdlib.h>
#include <stdio.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

int main(void)
{
    char line[1024];
    int most = 0;
    int cur = 0;
    int cals;

    while (fgets(line, sizeof line, stdin))
    {
        if (sscanf(line, "%d", &cals) == 1)
        {
            cur += cals;
        }
        else
        {
            most = MAX(cur, most);
            cur = 0;
        }
    }
    most = MAX(cur, most);
    printf("%d\n", most);
    return EXIT_SUCCESS;
}
