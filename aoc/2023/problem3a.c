// gcc -Werror -pedantic -std=c99 -Wall -Wextra -O2 -o problem3a problem3a.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool xisdigit(char c)
{
    return c >= '0' && c <= '9';
}

static bool xisdot(char c)
{
    return c == '.' || c == '\0' || c == '\n';
}

int main(void)
{
    static char grid[1024][1024];
    int height;
    int width;
    int sum;

    for (height = 1; fgets(grid[height] + 1, sizeof grid[height], stdin); height++);
    height--;
    width = (int) strlen(grid[1] + 1) - 1;
                
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int num = 0;
            bool valid = false;
            
            while (xisdigit(grid[i][j]))
            {
                num *= 10;
                num += grid[i][j] - '0';
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        if (!xisdigit(grid[i + x][j + y])
                            && !xisdot(grid[i + x][j + y]))
                        {
                            valid = true;
                        }
                    }
                }
                j++;
            }
            if (valid)
            {
                sum += num;
            }
        }
    }
    printf("%u\n", sum);
    return EXIT_SUCCESS;
}
