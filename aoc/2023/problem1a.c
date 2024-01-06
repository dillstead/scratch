//  gcc -Werror -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem1a problem1a.c

#include <stdlib.h>

static bool xisdigit(char c)
{
    return c>='0' && c<='9';
}

int main(void)
{
    char line[256];
    
    while (fgets(line, sizeof line, stdin))
    {
        size_t len = strlen(line);
        for (size_t i = 0; i < len; i++)
        {
            
        }
        for (size_t i = len - 1; i >= 0; i--)
        {
            
        }
    }
    
    return EXIT_SUCCESS;
}
