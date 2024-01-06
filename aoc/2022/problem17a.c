// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem17a problem17a.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#define MAX_COL    9
#define MAX_ROW    (1 << 16)
#define BOTTOM     (MAX_ROW - 1)
#define ROCK_TYPES 5
#define MAX_STONES 5
#define MAX_ROCKS  2022

struct point
{
    int x;
    int y;
};

struct rock
{
    struct point stones[MAX_STONES];
    int num_stones;
    int top;
};

static struct rock rocks[ROCK_TYPES] =
{
    { { {BOTTOM, 3}, {BOTTOM, 4}, {BOTTOM, 5}, {BOTTOM, 6} }, 4, 3 },
    { { {BOTTOM, 4}, {BOTTOM - 1, 3}, {BOTTOM - 1, 4}, {BOTTOM - 1, 5}, { BOTTOM - 2, 4} }, 5, 4 },
    { { {BOTTOM, 3}, {BOTTOM, 4}, {BOTTOM, 5}, {BOTTOM - 1, 5}, {BOTTOM - 2, 5} }, 5, 4 },
    { { {BOTTOM, 3}, {BOTTOM - 1, 3}, {BOTTOM - 2, 3}, {BOTTOM - 3, 3} }, 4, 3 },
    { { {BOTTOM, 3}, {BOTTOM, 4}, {BOTTOM - 1, 3}, {BOTTOM - 1, 4} }, 4, 3 }
};

static bool grid[MAX_ROW][MAX_COL];

static void stamp(struct rock *rock)
{
    for (int i = 0; i < rock->num_stones; i++)
    {
        grid[rock->stones[i].x][rock->stones[i].y] = true;
    }
}
        
static bool move(struct rock *rock, struct point *point)
{
    struct rock new_rock = *rock;

    for (int i = 0; i < new_rock.num_stones; i++)
    {
        new_rock.stones[i].x += point->x;
        new_rock.stones[i].y += point->y;
        if (grid[new_rock.stones[i].x][new_rock.stones[i].y])
        {
            return false;
        }
    }
    *rock = new_rock;
    return true;
}

int main(void)
{
    char jets[(1 << 10) * 10];
    int jet_len;
    int jet_pos = 0;
    int rock_pos = 0;
    int bottom = BOTTOM;
    struct rock rock;

    for (int i = 0; i < MAX_ROW; i++)
    {
        grid[i][0] = true;
        grid[i][MAX_COL - 1] = true;
    }
    for (int i = 0; i < MAX_COL; i++)
    {
        grid[BOTTOM][i] = true;
    }
        
    fgets(jets, sizeof jets, stdin);
    jet_len = strlen(jets) - 1;
    
    for (int i = 0; i < MAX_ROCKS; i++)
    {
        rock = rocks[rock_pos];
        move(&rock, &(struct point) { -(BOTTOM - (bottom - 4)), 0 });
        do
        {
            if (jets[jet_pos] == '<')
            {
                move(&rock, &(struct point) { 0, -1 });
            }
            else
            {
                move(&rock, &(struct point) { 0, 1 });
            }
            jet_pos = (jet_pos + 1) % jet_len;
        } while (move(&rock, &(struct point) { 1, 0 }));
        stamp(&rock);
        bottom = MIN(bottom, rock.stones[rock.top].x);
        rock_pos = (rock_pos + 1) % ROCK_TYPES;
    }
    printf("%d\n", BOTTOM - bottom);
    return EXIT_SUCCESS;
}
