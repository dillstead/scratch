// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem6a problem6a.c
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1 << (x - 'a'))

static int bitcnt(uint32_t bits)
{
    int cnt = 0;
    
    while (bits)
    {
        cnt += bits & 0x1;
        bits = bits >> 1;
    }
    return cnt;
}

int main(void)
{
    char line[1024 * 16];

    while (fgets(line, sizeof line, stdin))
    {
        for (int i = 3; line[i] != '\n'; i++)
        {
            if (bitcnt(BIT(line[i]) | BIT(line[i - 1]) | BIT(line[i - 2]) | BIT(line[i - 3])) == 4)
            {
                printf("%d\n", i + 1);
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
