// gcc -Werror -std=c99 -Wall -Wextra -O2 -o xuitoa xuitoa.c
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define duitoa(i,s) uitoa((i),(s),10,"0123456789")
#define xuitoa(i,s) uitoa((i),(s),16,"0123456789abcdef")

static char *uitoa(unsigned int i, char *s, int base, const char *digits)
{
    int len = 1;
    unsigned int t = i;
    while (i /= base) len++;
    char *beg = s + len;
    *beg = '\0';
    do
    {
        *--beg = digits[t % base];
    } while (t /= base);
    return s;
}

int main(void)
{
    char buf[64];
    assert(!strcmp(duitoa(8, buf), "8"));
    assert(!strcmp(duitoa(0, buf), "0"));
    assert(!strcmp(duitoa(87658, buf), "87658"));
    assert(!strcmp(duitoa(2147483647, buf), "2147483647"));
    assert(!strcmp(duitoa(3204448205, buf), "3204448205"));
    assert(!strcmp(xuitoa(8, buf), "8"));
    assert(!strcmp(xuitoa(0, buf), "0"));
    assert(!strcmp(xuitoa(87658, buf), "1566a"));
    assert(!strcmp(xuitoa(2147483647, buf), "7fffffff"));
    assert(!strcmp(xuitoa(0xbeffffcd, buf), "beffffcd"));
    return EXIT_SUCCESS;
}
