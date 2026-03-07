#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

static uint32_t rand_val(void)
{
    return (uint32_t) rand() << 16 | (uint32_t) (rand() & 0xFFFF);
}

int main(void)
{
    srand((unsigned)time(NULL));

    while (1)
    {
        uint32_t val = rand_val();
        uint32_t map = 0;
        uint32_t tmp = val;
        int i = 0;

        for (; i < 32; i++)
        {
            uint32_t pos = 1U << ((tmp & 0xF8000000) >> 27);
            if ((pos & map) != 0)
            {
                break;
            }
            map |= pos;
            tmp <<= 1;
        }
        //printf("done\n");
        if (i == 32)
        {
            assert(map == UINT32_MAX);
            printf("%x\n", val);
            break;
        }
    }
    return EXIT_SUCCESS;
}
