// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem22b problem22b.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CUBE_SIZE   50
#define NUM_DIRS    4
#define NUM_FACES   6
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))

enum direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

enum action
{
    CORNER,
    SAME_SIDE,
    OPP_SIDE
};

struct location
{
    int x;
    int y;
};

struct state
{
    struct location loc;
    int dir;
};

static struct location offsets[] =
{
    {1,  0},
    {0,  1},
    {-1, 0},
    {0, -1},
};

struct transition
{
    int id;
    enum direction dir;
    enum action act;
};

static int loc_id[CUBE_SIZE][CUBE_SIZE] =
{
    {0, 0, 1, 0},
    {0, 2, 0, 0},
    {3, 4, 0, 0},
    {5, 0, 0, 0}
};

static struct location id_loc[NUM_FACES] =
{
    {1, 0}, {2, 0}, {1, 1}, {0, 2}, {1, 2}, {0, 3}
};

static struct transition trans[NUM_FACES][NUM_DIRS] = 
{
    {{0}, {0}, {3, RIGHT, OPP_SIDE}, {5, RIGHT, CORNER}},
    {{4, LEFT, OPP_SIDE}, {2, LEFT, CORNER}, {0}, {5, UP, SAME_SIDE}},
    {{1, UP, CORNER}, {0}, {3, DOWN, CORNER}, {0}},
    {{0}, {0}, {0, RIGHT, OPP_SIDE}, {2, RIGHT, CORNER}},
    {{1, LEFT, OPP_SIDE}, {5, LEFT, CORNER}, {0}, {0}},
    {{4, UP, CORNER}, {1, DOWN, SAME_SIDE}, {0, DOWN, CORNER}, {0}}
};

static char map[CUBE_SIZE * NUM_FACES][CUBE_SIZE * NUM_FACES];

static int mod(int x, int y)
{
    int val = x % y;

    return (val >= 0 ? val : y + val);
}

static void warp(struct state *state, int *x, int *y, int *dir)
{
    struct transition tran = trans[loc_id[state->loc.y / CUBE_SIZE][state->loc.x / CUBE_SIZE]][state->dir];
    int ux = state->loc.x % CUBE_SIZE;
    int uy = state->loc.y % CUBE_SIZE;
    int t;

    if (tran.act == CORNER)
    {
        t = uy;
        if ((state->dir == UP && tran.dir == LEFT)
            || (state->dir == DOWN && tran.dir == RIGHT))
        {

            uy = (CUBE_SIZE - 1) - ux;
            ux = (t == 0 ? (CUBE_SIZE - 1) : 0);
        }
        else if ((state->dir == RIGHT && tran.dir == DOWN)
                 || (state->dir == LEFT && tran.dir == UP))
        {
            uy = (ux == 0 ? (CUBE_SIZE - 1) : 0);
            ux = (CUBE_SIZE - 1) - t;
        }
        else
        {
            uy = ux;
            ux = t;
        }
    }
    else if (tran.act == OPP_SIDE)
    {
        if ((state->dir == RIGHT && tran.dir == LEFT)
            || (state->dir == LEFT && tran.dir == RIGHT))
        {
            uy = (CUBE_SIZE - 1) - uy;
        }
        else
        {
            ux = (CUBE_SIZE - 1) - ux;
            
        }
    }
    else
    {
        if (state->dir == DOWN || state->dir == UP)
        {
            uy = (uy == 0 ? CUBE_SIZE - 1 : 0);
        }
        else
        {
            ux = (ux == 0 ? CUBE_SIZE - 1 : 0);
        }
    }
    *dir = tran.dir;
    *x = id_loc[tran.id].x * CUBE_SIZE + ux;
    *y = id_loc[tran.id].y * CUBE_SIZE + uy;
}

int main(void)
{
    char line[8192];
    unsigned int max_rows = 0;
    unsigned int max_cols = 0;
    unsigned int len;
    unsigned int total;
    int pos;
    int steps;
    int dir;
    int x;
    int y;
    char c;
    struct state state;

    memset(map, ' ', sizeof map);
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == '\n')
        {
            break;
        }
        len = strlen(line) - 1;
        max_cols = MAX(max_cols, len);
        memcpy(map[max_rows], line, len);
        max_rows++;
    }
    fgets(line, sizeof line, stdin);
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
    map[state.loc.y][state.loc.x] = '>';
    while (total < len)
    {
        if (sscanf(line + total, "%d%n", &steps, &pos) == 1)
        {
            x = state.loc.x;
            y = state.loc.y;
            dir = state.dir;
            while (steps > 0)
            {
                x += offsets[state.dir].x;
                y += offsets[state.dir].y;
                if (x < 0 || (unsigned int ) x == max_cols || y < 0 || (unsigned int) y == max_rows
                    || map[y][x] == ' ')
                {
                    warp(&state, &x, &y, &dir);
                }
                if (map[y][x] == '#')
                {
                    break;
                }
                state.loc.x = x;
                state.loc.y = y;
                state.dir = dir;
                set_dir(map, &state);
                steps--;
            }
            total += pos;
        }
        else
        {
            sscanf(line + total, "%c", &c);
            state.dir += (c == 'R' ? 1 : -1);
            state.dir = mod(state.dir, ARRAYLEN(offsets));
            set_dir(map, &state);
            total++;
        }
    }
    printf("%d\n", 1000 * (state.loc.y + 1) + 4 * (state.loc.x + 1) + state.dir);
    return EXIT_SUCCESS;
}
