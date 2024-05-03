// gcc -Werror -std=c11 -Wall -Wextra -O2 -I../types -I../arena -o rand_range ../arena/arena.c rand_range.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "arena.h"

static int randint(uint64_t *rng, int lo, int hi)
{
    *rng = *rng * 0x3243f6a8885a308d + 1;
    return (int) (((*rng>>32) * (hi - lo)) >>32) + lo;
}

static void generate(uint64_t seed, int *nums, int len, int lo, int hi)
{
    seed += 1111111111111111111u;
    for (int i = 0; i < len; i++)
    {
        nums[i] = randint(&seed, lo, hi);
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: rand_range <num> <low> <high>\n");
        return EXIT_FAILURE;
    }

    int max_nums = atoi(argv[1]);
    int low = atoi(argv[2]);
    int high = atoi(argv[3]);
    struct arena arena = new_arena(((size_t) max_nums * sizeof(int) << 2));
    int *nums = new(&arena, int, (size_t) max_nums);

    generate(time(NULL), nums, max_nums, low, high);
    for (int i = 0; i < max_nums; i++)
    {
        printf("%d\n", nums[i]);
    }
    return EXIT_SUCCESS;
}
