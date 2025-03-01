// gcc -Werror -std=c99 -Wall -Wextra -O2 -o xitoa xitoa.c
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static char *xitoa(int i, char *s)
{
    int len = 1;
    int t = i > 0 ? -i : i;
    if (i < 0)
    {
        *s = '-';
        len++;
    }
    while (i /= 10) len++;
    char *beg = s + len;
    *beg = '\0';
    do
    {
        *--beg = '0' - t % 10;
    } while (t /= 10);
    return s;
}

int main(void)
{
    char buf[64];
    assert(!strcmp(xitoa(0, buf), "0"));
    assert(!strcmp(xitoa(-1234, buf), "-1234"));
    assert(!strcmp(xitoa(87658, buf), "87658"));
    assert(!strcmp(xitoa(2147483647, buf), "2147483647"));
    assert(!strcmp(xitoa(-2147483648, buf), "-2147483648"));
    return EXIT_SUCCESS;
}
