// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -o problem15a problem15a.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define EXP        24
#define TARGET_ROW 2000000

uint64_t set[1 << EXP];

static uint64_t hash(int32_t x, int32_t y, size_t *mask, size_t *step)
{
    uint64_t h = (uint64_t) y << 32 | (uint32_t) x;

    h = (h - 0x8000000080000000) * 1111111111111111111;
    *mask = ((size_t) 1 << EXP) - 1;
    *step = (h >> (64 - EXP)) | 1;
    return h;
}

static bool add(int32_t x, int32_t y)
{
    size_t mask;
    size_t step;
    uint64_t h = hash(x, y, &mask, &step);
    
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

static void rmv(int32_t x, int32_t y)
{
    size_t mask;
    size_t step;
    uint64_t h = hash(x, y, &mask, &step);
    
    for (size_t i = h;;)
    {
        i = (i + step) & mask;
        if (!set[i])
        {
            return;
        }
        else if (set[i] == h)
        {
            set[i] = 0;
            return;
        }
    }
}

static unsigned int count(void)
{
    unsigned int count = 0;
    
    for (size_t i = 0; i < sizeof set / sizeof set[0]; i++)
    {
        if (set[i])
        {
            count++;
        }
    }
    return count;
}

static int check_target_row(int32_t sx, int32_t sy, int32_t d, int32_t row,
                            int32_t *tx, int32_t *ty)
{
    int32_t td = abs(sy - row);
    int32_t len;

    if (td > d)
    {
        return 0;
    }
    *tx = sx - d + td;
    *ty = row;
    len = (d * 2 + 1) - (td * 2);
    return len;
}

int main(void)
{
    int32_t sx, sy;
    int32_t bx, by;
    int32_t tx, ty;
    int len;

    while (fscanf(stdin, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n",
                  &sx, &sy, &bx, &by) == 4)
    {
        len = check_target_row(sx, sy, abs(bx - sx) + abs(by - sy), TARGET_ROW, &tx, &ty);
        for (int i = 0; i < len; i++)
        {
            add(tx + i, ty);
        }
        if (by == TARGET_ROW)
        {
            rmv(bx, by);
        }
    }
    printf("%u\n", count());
    return EXIT_SUCCESS;
}
