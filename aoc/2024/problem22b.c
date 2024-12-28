// gcc -std=c99 -O2 -o problem22b problem22b.c
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define EXP        24

static uint64_t set[1 << EXP];

static uint64_t hash(uint16_t a, uint16_t b, uint16_t c, uint16_t d, size_t *mask, size_t *step)
{
    uint64_t h = (uint64_t) a << 48
        | (uint64_t) b << 32
        | (uint64_t) c << 16
        | (uint64_t) d;

    h = (h - 0x8000800080008000) * 1111111111111111111;
    *mask = ((size_t) 1 << EXP) - 1;
    *step = (h >> (64 - EXP)) | 1;
    return h;
}

static bool add(int16_t a, int16_t b, int16_t c, int16_t d)
{
    size_t mask;
    size_t step;
    uint64_t h = hash(a, b, c, d, &mask, &step);

    for (size_t i = h;;)
    {
        i = (i + step) & mask;
        if (!set[i])
        {
            set[i] = h;
            return true;
        }
        else if (set[i] == h)
        {
            return false;
        }
    }    
}

int main(void)
{
    char line[1024];
    
    while (fgets(line, sizeof line, stdin))
    {
        uint32_t code;
        if (sscanf(line, "%" PRIu32, &code) != 1)
        {
            continue;
        }
        
        uint32_t save[2001];
        save[0] = code;
        for (int i = 1; i <= 2000; i++)
        {
            code ^= code << 6;
            code &= 0xFFFFFF;
            code ^= code >> 5;
            code &= 0xFFFFFF;
            code ^= code << 11;
            code &= 0xFFFFFF;
            save[i] = code;
        }
        memset(set, 0, sizeof set);
        for (int i = 4; i <= 2000; i++)
        {
            if (add(save[i - 3] % 10 - save[i - 4] % 10,
                     save[i - 2] % 10 - save[i - 3] % 10,
                     save[i - 1] % 10 - save[i - 2] % 10,
                     save[i] % 10 - save[i - 1] % 10))
            {
                printf("%"PRIu32": ", save[i] % 10);
                for (int j = i - 3; j <= i; j++)
                {
                    printf("%"PRId32",", (int32_t) (save[j] % 10 - save[j - 1] % 10));
                }
                printf("\n");
            }
        }
    }
    return EXIT_SUCCESS;
}   
