// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem3a problem3a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void)
{
    bool items[256] = { 0 };
    int pri[256] = { 0 };
    char line[256];
    int sum = 0;

    for (int i = 'A'; i <= 'Z'; i++)
    {
        pri[i] = i - 'A' + 27;
    }
    for (int i = 'a'; i <= 'z'; i++)
    {
        pri[i] = i - 'a' + 1;
    }
    while (fgets(line, sizeof line, stdin))
    {
        int mid = strlen(line) / 2;
        
        for (int i = 0; i < mid; i++)
        {
            items[(unsigned char) line[i]] = true;
        }
        for (int i = mid; line[i] != '\n'; i++)
        {
            if (items[(unsigned char) line[i]])
            {
                sum += pri[(unsigned char )line[i]];
                memset(items, 0, sizeof items);
                break;
            }
        }
    }
    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
