// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem18a problem18a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_POINTS 3000

struct point
{
    int x;
    int y;
    int z;
};
    
static bool grid[50][50][50];
static struct point points[MAX_POINTS];

int main(void)
{
    int x, y, z;
    int area = 0;
    int num_points = 0;
    int offsets[] = { -1, 1 };
    
    while (scanf("%d,%d,%d\n", &x, &y, &z) == 3)
    {
        points[num_points++] = (struct point) { x + 1, y + 1, z + 1 };
        grid[x + 1][y + 1][z + 1] = true;
    }
    for (int i = 0; i < num_points; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            area += !grid[points[i].x + offsets[j]][points[i].y][points[i].z];
            area += !grid[points[i].x][points[i].y + offsets[j]][points[i].z];
            area += !grid[points[i].x][points[i].y][points[i].z + offsets[j]];
        }
    }
    printf("%d\n", area);
    return EXIT_SUCCESS;
}
