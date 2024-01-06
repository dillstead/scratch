// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem14a problem14a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX_ROW   171
#define MAX_COL   61
#define COL_ADJ   458
#define START_COL (500 - COL_ADJ)
#define START_ROW 0

static bool map[MAX_ROW][MAX_COL];

static void draw_line(int x, int y, int x1, int y1)
{
    x -= COL_ADJ;
    x1 -= COL_ADJ;
    if (x == x1)
    {
        for (int i = MIN(y, y1); i <= MAX(y, y1); i++)
        {
            map[i][x] = true;
        }
    }
    else
    {
        for (int i = MIN(x, x1); i <= MAX(x, x1); i++)
        {
            map[y][i] = true;
        }
    }
}

int main(void)
{
    char line[256];
    int col_offs[] = { 0, -1, 1 };
    int x, y, x1, y1;
    int total;
    int pos;
    int step = -1;

    memset(map, 0, sizeof map);
    while (fgets(line, sizeof line, stdin))
    {
        sscanf(line, "%d,%d%n", &x, &y, &total);
        while (sscanf(line + total, " -> %d,%d%n", &x1, &y1, &pos) == 2)
        {
            draw_line(x, y, x1, y1);
            x = x1;
            y = y1;
            total += pos;
        }
    }
    do
    {
        x = START_COL;
        y = START_ROW;
        map[y][x] = true;
        do
        {
            y1 = y + 1;
            for (unsigned int i = 0; i < sizeof col_offs / sizeof col_offs[0]; i++)
            {
                x1 = x + col_offs[i];
                if (!map[y1][x1])
                {
                    map[y][x] = false;
                    map[y1][x1] = true;
                    x = x1;
                    y = y1;
                    break;
                }
            }
        } while (x == x1 && y == y1
                 && x > 1 && x < MAX_COL - 1 && y < MAX_ROW - 1);
        step++;
    } while (x > 1 && x < MAX_COL - 1 && y < MAX_ROW - 1);
    printf("%d\n", step);
    return EXIT_SUCCESS;
}
