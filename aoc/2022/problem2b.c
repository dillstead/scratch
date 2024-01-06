// gcc -Werror -funsigned-char -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem2b problem2b.c
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int rps[3][3] = { { 3, 1, 2 }, { 4, 5, 6 }, { 8, 9, 7} };
    char you;
    char them;
    int score = 0;

    while (fscanf(stdin, "%c %c\n", &them, &you) == 2)
    {
        score += rps[you - 'X'][them - 'A'];
    }
    printf("%d\n", score);
    return EXIT_SUCCESS;
}
