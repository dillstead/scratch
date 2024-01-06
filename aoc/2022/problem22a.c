// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem22a problem22a.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))

struct location
{
    int x;
    int y;
};

struct state
{
    struct location loc;
    int turn;
};

static struct location offsets[] =
{
    {  1,  0 },
    {  0,  1 },
    { -1,  0 },
    {  0, -1 },
};

static int mod(int x, int y)
{
    int val = x % y;

    return (val >= 0 ? val : y + val);
}

int main(int argc, char **argv)
{
    (void) argc;
    char line[8192];
    char **map;
    FILE *fin;
    unsigned int max_rows = 0;
    unsigned int max_cols = 0;
    unsigned int len;
    bool insert = false;
    unsigned int total;
    int pos;
    int steps;
    char turn;
    int x;
    int y;
    struct state state;

    fin = fopen(argv[1], "r");
    if (fin == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
read_map:    
    while (fgets(line, sizeof line, fin))
    {
        if (line[0] == '\n')
        {
            if (insert)
            {
                break;
            }
            map = malloc(max_rows * sizeof *map);
            for (unsigned int i = 0; i < max_rows; i++)
            {
                map[i] = malloc(max_cols);
                memset(map[i], ' ', max_cols);
            }
            fseek(fin, 0, SEEK_SET);
            max_rows = 0;
            insert = true;
            goto read_map;
        }
        len = strlen(line) - 1;
        max_cols = MAX(max_cols, len);
        if (insert)
        {
            memcpy(map[max_rows], line, len);
        }
        max_rows++;
    }
    fgets(line, sizeof line, fin);
    len = strlen(line);
    line[len-- - 1] = '\0';
    total = 0;
    memset(&state, 0, sizeof state);
    for (; (unsigned int) state.loc.x < max_cols; state.loc.x++)
    {
        if (map[0][state.loc.x] == '.')
        {
            break;
        }
    }
    while (total < len)
    {
        if (sscanf(line + total, "%d%n", &steps, &pos) == 1)
        {
            x = state.loc.x;
            y = state.loc.y;
            while (steps > 0)
            {
                x += offsets[state.turn].x;
                x = mod(x, max_cols);
                y += offsets[state.turn].y;
                y = mod(y, max_rows);
                if (map[y][x] == '#')
                {
                    break;
                }
                if (map[y][x] == '.')
                {
                    steps--;
                    state.loc.x = x;
                    state.loc.y = y;
                }
            }
            total += pos;
        }
        else
        {
            sscanf(line + total, "%c", &turn);
            state.turn += (turn == 'R' ? 1 : -1);
            state.turn = mod(state.turn, ARRAYLEN(offsets));
            total++;
        }
    }
    fclose(fin);
    printf("%d\n", 1000 * (state.loc.y + 1) + 4 * (state.loc.x + 1) + state.turn);
    return EXIT_SUCCESS;
}
