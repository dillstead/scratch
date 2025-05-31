// gcc -std=c99 -fsanitize-undefined-trap-on-error -Werror -Wall -Wconversion -Wextra -O2 -o idiv idiv.c
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

static void uidivmod(unsigned int n, unsigned int d, unsigned int *q, unsigned int *r)
{
    *q = 0;
    *r = 0;
    if (d == 0)
    {
        return;
    }

    for (int i = 31; i >= 0; i--)
    {
        if (d <= (n >> i))
        {
            n -= (d << i);
            (*q) |= 0x1U << i;
        }
    }
    *r = n;
}

static int idiv(int n, int d)
{
    if (n == 0 || d == 0)
    {
        return 0;
    }

    if (n == d)
    {
        return 1;
    }

    if (d == INT_MIN)
    {
        return 0;
    }

    int s = (n >> 31) ^ (d >> 31) ? -1 : 1;
    unsigned int _d = (d < 0) ? (unsigned int) -d : (unsigned int) d;
    unsigned int _n = (unsigned int) n;
    if (n < 0)
    {
        _n = (n == INT_MIN) ? (unsigned int) INT_MAX : (unsigned int) -n;
    }

    unsigned int q;
    unsigned int r;
    uidivmod(_n, _d, &q, &r);
    if (n == INT_MIN && r + 1 == _d)
    {
        q++;
    }
    return s * (int) q;
}

int main(void)
{
    assert(INT_MIN / 1 == idiv(INT_MIN, 1));
    assert(INT_MIN / 2 == idiv(INT_MIN, 2));
    assert(INT_MIN / 3 == idiv(INT_MIN, 3));
    assert(INT_MIN / -2 == idiv(INT_MIN, -2));
    assert(INT_MIN / -3 == idiv(INT_MIN, -3));
    assert(INT_MAX / 1 == idiv(INT_MAX, 1));
    assert(INT_MAX / 2 == idiv(INT_MAX, 2));
    assert(INT_MAX / 3 == idiv(INT_MAX, 3));
    assert(INT_MAX / -1 == idiv(INT_MAX, -1));
    assert(INT_MAX / -2 == idiv(INT_MAX, -2));
    assert(INT_MAX / -3 == idiv(INT_MAX, -3));
    assert(0 / -3 == idiv(0, -3));
    assert(-3 / -3 == idiv(-3, -3));
    assert(INT_MIN / INT_MIN == idiv(INT_MIN, INT_MIN));
    assert(INT_MAX / INT_MAX == idiv(INT_MAX, INT_MAX));
    return EXIT_SUCCESS;
}
