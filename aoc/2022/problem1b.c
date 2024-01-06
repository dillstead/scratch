// gcc -Werror -funsigned-char -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem1b problem1b.c
#include <stdlib.h>
#include <stdio.h>

static int comp(const void *a, const void *b)
{
    return *((int *) a) - *((int *) b);
}

static void insert(int *most, int cals)
{
    if (cals > most[0])
    {
        most[0] = cals;
        qsort(most, 3, sizeof *most, comp);
    }
}

int main(void)
{
    char line[1024];
    int most[3] = { 0 };
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
            insert(most, cur);
            cur = 0;
        }
    }
    insert(most, cur);
    printf("%d\n", most[0] + most[1] + most[2]);
    return EXIT_SUCCESS;
}
