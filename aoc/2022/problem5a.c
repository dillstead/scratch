// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem5a problem5a.c
#include <stdlib.h>
#include <stdio.h>

static void push(char *stack, int *top, char c)
{
    stack[(*top)++] = c;
}

static char pop(char *stack, int *top)
{
    return stack[--*top];
}

int main(void)
{
    char stacks[9][56] =
    {
        { 'R', 'N', 'F', 'V', 'L', 'J', 'S', 'M', },
        { 'P', 'N', 'D', 'Z', 'F', 'J', 'W', 'H', },
        { 'W', 'R', 'C', 'D', 'G' },
        { 'N', 'B', 'S' },
        { 'M', 'Z', 'W', 'P', 'C', 'B', 'F', 'N' },
        { 'P', 'R', 'M', 'W' },
        { 'R', 'T', 'N', 'G', 'L', 'S', 'W' },
        { 'Q', 'T', 'H', 'F', 'N', 'B', 'V' },
        { 'L', 'M', 'H', 'Z', 'N', 'F' },
    };
    int tops[9] = { 8, 8, 5, 3, 8, 4, 7, 7, 6 };
    int num, from, to;

    while (scanf("move %d from %d to %d\n", &num, &from, &to) == 3)
    {
        for (int i = 0; i < num; i++)
        {            
            push(stacks[to - 1], &tops[to - 1], pop(stacks[from - 1], &tops[from - 1]));
        }
    }
    for (int i = 0; i < 9; i++)
    {
        printf("%c", pop(stacks[i], &tops[i]));
    }
    printf("\n");
    return EXIT_SUCCESS;
}
