#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static void generate(int n)
{
    int *p = calloc(n, sizeof *p);
    int *x = calloc(n * n, sizeof *x);
    int xi = 0;

    p[0] = 2;
    x[xi++] = 2;
    int k = 1;
    for (int j = 3; k < n; j += 2)
    {
        bool jprime = false;
        int square = p[xi] * p[xi];
        if (j == square)
        {
            x[xi++] = j;
        }
        else
        {
            jprime = true;
            for (int mi = 1; mi < xi; mi++)
            {
                int m = x[mi];
                while (m < j)
                {
                    m += 2 * p[mi];
                }
                x[mi] = m;
                if (j == m)
                {
                    jprime = false;
                    break;
                }
            }
        }
        if (jprime)
        {
            p[k++] = j;
        }
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", p[i]);
    }
}

int main(int argc, char **argv)
{
    generate(atoi(argv[1]));
    return EXIT_SUCCESS;
}
