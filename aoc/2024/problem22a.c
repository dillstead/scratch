// gcc -std=c99 -O2 -o problem22a problem22a.c
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    char line[1024];
    uint64_t sum = 0;
    
    while (fgets(line, sizeof line, stdin))
    {
        uint32_t code;
        if (sscanf(line, "%" PRIu32, &code) != 1)
        {
            continue;
        }
        for (int i = 0; i < 2000; i++)
        {
            code ^= code << 6;
            code &= 0xFFFFFF;
            code ^= code >> 5;
            code &= 0xFFFFFF;
            code ^= code << 11;
            code &= 0xFFFFFF;
        }
        sum += code;
    }
    printf("%"PRIu64"\n", sum);
    return EXIT_SUCCESS;
}   
