// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=address,undefined -g3 -o problem15a problem15a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static char line[1 << 15];

int main(void)
{
    int i = 0;
    int sum = 0;
    int hash = 0;
    
    fgets(line, sizeof line, stdin);
    while (line[i])
    {
        if (line[i] == ',' || line[i] == '\n')
        {
            sum += hash;
            hash = 0;
        }
        else
        {
            hash += line[i];
            hash *= 17;
            hash &= 256 - 1;
        }
        i++;
    }
    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
    
