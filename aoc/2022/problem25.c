// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=address,undefined -g3 -o problem25 problem25.c
// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -O2 -o problem25 problem25.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PSUM_OFFSET  6
#define DIGIT_OFFSET 2
#define MAX_DIGITS   32
#define ARRAYLEN(x)  (sizeof(x) / sizeof(x[0]))

struct snafu
{
    char digits[MAX_DIGITS];
};

static char char_to_dig_map[] =
{
    ['='] = -2, ['-'] = -1, ['0'] = 0, ['1'] = 1, ['2'] = 2
};

static char dig_to_char_map[] =
{
    '=', '-', '0', '1', '2'
};

struct partial_sum
{
    char digit;
    char carry;
};

static struct partial_sum psums[] =
{
    {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0},
    {1, 0}, {2, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, 
};
    

static void str_to_snafu(struct snafu *s, char *str)
{
    int len = (int) strlen(str);
    for (int i = len - 1, j = 0; i >= 0; i--, j++)
    {
        s->digits[j] = char_to_dig_map[(int) str[i]];
    }
}

static void snafu_to_str(char *str, struct snafu *s)
{
    int i;

    for (i = (int) ARRAYLEN(s->digits) - 1; i >= 0; i--)
    {
        if (s->digits[i] != 0)
        {
            break;
        }
    }
    if (i < 0)
    {
        *str++ = '0';
        *str = '\0';
        return;
    }
    for (; i >=0; i--)
    {
        *str++ = dig_to_char_map[s->digits[i] + DIGIT_OFFSET];
    }
    *str = '\0';
}

static void sum_snafu(struct snafu *x, struct snafu *y)
{
    int carry = 0;
    
    for (int i = 0; i < (int) ARRAYLEN(x->digits) - 1; i++)
    {
        struct partial_sum psum = psums[(x->digits[i] + y->digits[i] + carry) + PSUM_OFFSET];
        y->digits[i] = psum.digit;
        carry = psum.carry;
    }
}

int main(void)
{
    char line[256];
    char str[33];
    struct snafu x;
    struct snafu sum;

    fgets(line, sizeof line, stdin);
    line[strcspn(line, "\n")] = '\0';
    memset(&sum, 0, sizeof x);
    str_to_snafu(&sum, line);
    while (fgets(line, sizeof line, stdin))
    {
        line[strcspn(line, "\n")] = '\0';
        memset(&x, 0, sizeof x);
        str_to_snafu(&x, line);
        sum_snafu(&x, &sum);
    }
    snafu_to_str(str, &sum);
    printf("%s\n", str);
    return EXIT_SUCCESS;
}
