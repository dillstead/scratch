// gcc -Werror -std=c99 -Wall -Wextra -O2 -o xatoi xatoi.c
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

static int xatoi(const char *s)
{
    const char *beg = s;
    int sn = 1;
    if (*beg == '-')
    {
        sn = -1;
        beg++;
    }
    
    int res = 0;
    while (*beg)
    {
        int dig = *beg - '0';
        if (sn == 1)
        {
            if (res > INT_MAX / 10
                || (res == INT_MAX / 10 && dig > INT_MAX % 10))
            {
                return INT_MAX;
            }
            res *= 10;
            res += dig;
        }
        else
        {
            if (res < INT_MIN / 10
                || (res == INT_MIN / 10 && -dig < INT_MIN % 10))
            {
                return INT_MIN;
            }
            res *= 10;
            res -= dig;
        }
        beg++;
    }
    return res;
}

int main(void)
{
    assert(xatoi("0") == 0);
    assert(xatoi("-1234") == -1234);
    assert(xatoi("87658") == 87658);
    assert(xatoi("2147483647") == 2147483647);
    assert(xatoi("2147483648") == 2147483647);
    assert(xatoi("-2147483648") == -2147483648);
    assert(xatoi("-2147483649") == -2147483648);
    return EXIT_SUCCESS;
}
