// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem6b problem6b.co
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
    uint32_t bits;

    while (fgets(line, sizeof line, stdin))
    {
        for (int i = 13; line[i] != '\n'; i++)
        {
            bits = 0;
            for (int j = 0; j < 14; j++)
            {
                bits |= BIT(line[i - j]);
            }
            if (bitcnt(bits) == 14)
            {
                printf("%d\n", i + 1);
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
