// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem11a problem11a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define NUM_ROUNDS  10000
#define MAX_QUEUE   (1u << 6)
#define NUM_MONKEYS 8

struct monkey
{
    int head;
    int tail;
    unsigned long queue[MAX_QUEUE];
    unsigned long inspect;
    unsigned long (*op)(unsigned long);
    int (*test)(unsigned long);
};

static int comp(const void *a, const void *b)
{
    return ((struct monkey *) b)->inspect - ((struct monkey *) a)->inspect;
}

static unsigned long monkey0_op(unsigned long old)
{
    return old * 5;
}

static unsigned long monkey1_op(unsigned long old)
{
    return old + 3;
}

static unsigned long monkey2_op(unsigned long old)
{
    return old + 7;
}

static unsigned long monkey3_op(unsigned long old)
{
    return old + 5;
}

static unsigned long monkey4_op(unsigned long old)
{
    return old + 2;
}

static unsigned long monkey5_op(unsigned long old)
{
    return old * 19;
}

static unsigned long monkey6_op(unsigned long old)
{
    return old * old;
}

static unsigned long monkey7_op(unsigned long old)
{
    return old + 4;
}

static int monkey0_test(unsigned long item)
{
    return (item % 17) ? 7 : 4;
}

static int monkey1_test(unsigned long item)
{
    return (item % 7) ? 2 : 3;
}

static int monkey2_test(unsigned long item)
{
    return (item % 13) ? 7 : 0;
}

static int monkey3_test(unsigned long item)
{
    return (item % 2) ? 2 : 0;
}

static int monkey4_test(unsigned long item)
{
    return (item % 19) ? 5 : 6;
}

static int monkey5_test(unsigned long item)
{
    return (item % 3) ? 1 : 6;
}

static int monkey6_test(unsigned long item)
{
    return (item % 5) ? 1 : 3;
}

static int monkey7_test(unsigned long item)
{
    return (item % 11) ? 4 : 5;
}
        
static void push(struct monkey *monkey, unsigned long item)
{
    monkey->queue[monkey->head] = item;
    monkey->head = (monkey->head + 1) & (MAX_QUEUE - 1);
}

static unsigned long pop(struct monkey *monkey)
{
    unsigned long item;

    item = monkey->queue[monkey->tail];
    monkey->tail = (monkey->tail + 1) & (MAX_QUEUE - 1);
    return item;
}

static bool empty(struct monkey *monkey)
{
    return monkey->head == monkey->tail;
}
    
int main(void)
{
    unsigned long item;

    struct monkey monkeys[NUM_MONKEYS] =
    {
        { 2, 0, { 89, 74 }, 0, monkey0_op, monkey0_test },
        { 8, 0, { 75, 69, 87, 57, 84, 90, 66, 50 }, 0, monkey1_op, monkey1_test },
        { 1, 0, { 55 }, 0, monkey2_op, monkey2_test },
        { 5, 0, { 69, 82, 69, 56, 68 }, 0, monkey3_op, monkey3_test },
        { 3, 0, { 72, 97, 50 }, 0, monkey4_op, monkey4_test },
        { 6, 0, { 90, 84, 56, 92, 91, 91 }, 0, monkey5_op, monkey5_test },
        { 4, 0, { 63, 93, 55, 53 }, 0, monkey6_op, monkey6_test },
        { 7, 0, { 50, 61, 52, 58, 86, 68, 97 }, 0, monkey7_op, monkey7_test }
    };
    
    for (int i = 0; i < NUM_ROUNDS; i++)
    {
        for (int j = 0; j < NUM_MONKEYS; j++)
        {
            while (!empty(monkeys + j))
            {
                monkeys[j].inspect++;
                item = monkeys[j].op(pop(monkeys + j));
                push(monkeys + monkeys[j].test(item % 9699690), item % 9699690);
            }
        }
    }
    qsort(monkeys, NUM_MONKEYS, sizeof *monkeys, comp);
    printf("%lu\n", monkeys[0].inspect * monkeys[1].inspect);
    return EXIT_SUCCESS;
}
