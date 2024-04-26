// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=address,undefined -g3 -o delim_split delim_split.c
#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

static size_t delim_split(char *str, char delim, char **toks, size_t max_toks)
{
    char *start = str;
    char *end = strchr(str, '\0');
    size_t num_toks = 0;
    char nextc = '\0';

    memset(toks, 0, sizeof *toks * num_toks);
    while (start < end  && num_toks < max_toks)
    {
        char *next = strchrnul(start, delim);
        nextc = *next;
        *next = '\0';
        toks[num_toks++] = start;
        start = next + 1;
    }
    if (nextc == delim && num_toks < max_toks)
    {
        toks[num_toks++] = "\0";
    }
    return num_toks;
}

static bool check_toks(char **toks, const char **expected, size_t num_toks)
{
    for (size_t i = 0; i < num_toks; i++)
    {
        if (((!toks[i] || !expected[i]) && toks[i] != expected[i])
            || strcmp(toks[i], expected[i]))
        {
            return false;
        }
    }
    return true;
}

int main(void)
{
    char *toks[3];

    assert(delim_split(strdup(""), ':', toks, 0) == 0);
    assert(delim_split(strdup("x"), ':', toks, 0) == 0);
    assert(delim_split(strdup(":"), ':', toks, 0) == 0);
    assert(delim_split(strdup("x:"), ':', toks, 0) == 0);
    assert(delim_split(strdup(":x"), ':', toks, 0) == 0);
    assert(delim_split(strdup("x:x"), ':', toks, 0) == 0);
    
    assert(delim_split(strdup(""), ':', toks, 1) == 0);
    assert(delim_split(strdup("x"), ':', toks, 1) == 1);
    assert(check_toks(toks, (const char *[]) { "x" }, 1));
    assert(delim_split(strdup(":"), ':', toks, 1) == 1);
    assert(check_toks(toks, (const char *[]) { "\0" }, 1));
    assert(delim_split(strdup("x:"), ':', toks, 1) == 1);
    assert(check_toks(toks, (const char *[]) { "x" }, 1));
    assert(delim_split(strdup(":x"), ':', toks, 1) == 1);
    assert(check_toks(toks, (const char *[]) { "\0" }, 1));
    assert(delim_split(strdup("x:x"), ':', toks, 1) == 1);
    assert(check_toks(toks, (const char *[]) { "x" }, 1));
    
    assert(delim_split(strdup(""), ':', toks, 2) == 0);
    assert(delim_split(strdup("x"), ':', toks, 2) == 1);
    assert(check_toks(toks, (const char *[]) { "x" }, 1));
    assert(delim_split(strdup(":"), ':', toks, 2) == 2);
    assert(check_toks(toks, (const char *[]) { "\0", "\0" }, 2));
    assert(delim_split(strdup("x:"), ':', toks, 2) == 2);
    assert(check_toks(toks, (const char *[]) { "x", "\0" }, 2));
    assert(delim_split(strdup(":x"), ':', toks, 2) == 2);
    assert(check_toks(toks, (const char *[]) { "\0", "x" }, 2));
    assert(delim_split(strdup("x:x"), ':', toks, 2) == 2);
    assert(check_toks(toks, (const char *[]) { "x", "x" }, 2));

    assert(delim_split(strdup("::"), ':', toks, 3) == 3);
    assert(check_toks(toks, (const char *[]) { "\0", "\0", "\0" }, 3));
    
    return EXIT_SUCCESS;
}

