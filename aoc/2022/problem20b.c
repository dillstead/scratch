// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem20b problem20b.c
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_NUMBERS    6000
#define ENCRYPTION_KEY 811589153

struct number
{
    long val;
    int idx;
};

static int mod(long x, int y)
{
    int val = x % y;

    return (val >= 0 ? val : y + val);
}

static void move(struct number *nums, int nums_len, int *idxs, int idx)
{
    int num_idx = idxs[idx];
    long num_moves = nums[num_idx].val;
    int next_num_idx;
    int inc = 1;
    struct number target_num = nums[num_idx];

    if (num_moves < 0)
    {
        inc = -1;
        num_moves = -num_moves;
    }
    num_moves = mod(num_moves, nums_len - 1);
    
    for (long i = 0; i < num_moves; i++)
    {
        next_num_idx = mod(num_idx + inc, nums_len);
        nums[num_idx] = nums[next_num_idx];
        idxs[nums[next_num_idx].idx] = num_idx;
        num_idx = next_num_idx;
    }
    idxs[idx] = num_idx;
    nums[num_idx] = target_num;
}

int main(void)
{
    static struct number nums[MAX_NUMBERS];
    static int idxs[MAX_NUMBERS];
    int nums_len = 0;

    while (scanf("%ld\n", &nums[nums_len].val) == 1)
    {
        nums[nums_len].val *= ENCRYPTION_KEY;
        nums[nums_len].idx = nums_len;
        idxs[nums_len] = nums_len;
        nums_len++;
    }
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < nums_len; i++)
        {
            move(nums, nums_len, idxs, i);
        }
    }
    for (int i = 0; i < nums_len; i++)
    {
        if (nums[i].val == 0)
        {
            printf("%ld\n", nums[mod(i + 1000, nums_len)].val +
                   nums[mod(i + 2000, nums_len)].val +
                   nums[mod(i + 3000, nums_len)].val);
            break;
        }
    }
    return EXIT_SUCCESS;
}
