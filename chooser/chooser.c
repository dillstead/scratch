// gcc -Werror -pedantic -std=c11 -Wall -Wextra -O2 -I../arena -o chooser ../arena/arena.c chooser.c
#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

static int count;

static void choose(int *choices, int choice_idx, int max_choices, int *nums, int num_idx, int max_nums)
{
    if (choice_idx == max_choices)
    {
        for (int i = 0; i < max_choices; i++)
        {
            printf("%d ", choices[i]);
        }
        printf("\n");
        count++;
        return;
    }
    if (num_idx == max_nums)
    {
        return;
    }
    choose(choices, choice_idx, max_choices, nums, num_idx + 1, max_nums);
    choices[choice_idx++] = nums[num_idx];
    choose(choices, choice_idx, max_choices, nums, num_idx + 1, max_nums);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: chooser <choices> <num>\n");
        return EXIT_FAILURE;
    }

    int max_choices = atoi(argv[1]);
    int max_nums = atoi(argv[2]);
    struct arena arena = new_arena(((size_t) max_nums * sizeof(int)) << 2);
    int *nums = new(&arena, int, (size_t) max_nums);

    for (int i = 0; i < max_nums; i++)
    {
        nums[i] = i + 1;
    }
    choose(new(&arena, int, (size_t) max_choices), 0, max_choices, nums, 0, max_nums);
    printf("%d\n", count);
    return EXIT_SUCCESS;
}
