// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem3b problem3b.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char items[256] = { 0 };
    char line[256];
    int cnt = 0;
    int sum = 0;

    while (fgets(line, sizeof line, stdin))
    {
        for (int i = 0; line[i] != '\n'; i++)
        {
            items[(int) line[i]] |= (1 << cnt);
        }
        if (++cnt == 3)
        {
            int c = (char *) memchr(items, 0x7, sizeof items) - items;
            sum += c >= 'a' ? c - 'a' + 1: c - 'A' + 27;
            memset(items, 0, sizeof items);
            cnt = 0;            
        }
    }
    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
