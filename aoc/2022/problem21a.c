// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem21a problem21a.c
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define EXP         12
#define MAX_NAME    16
#define MAX_MONKEYS (1 << EXP)

struct monkey
{
    char name[MAX_NAME];
    char op;
    long val;
    char left[MAX_NAME];
    char right[MAX_NAME];
};

struct htable
{
    struct monkey *entries[MAX_MONKEYS];
    int32_t num_entries;
};

static struct monkey *alloc_monkey(void)
{
    static int i = 0;
    static struct monkey monkeys[MAX_MONKEYS];

    return &monkeys[i++];
}

static uint64_t hash(const char *str, int len)
{
    uint64_t h = 0x100;
    
    for (int i = 0; i < len; i++)
    {
        h ^= str[i] & 255;
        h *= 1111111111111111111;
    }
    return h ^ h >> 32;
}    

static int32_t lookup(uint64_t hash, int exp, int32_t idx)
{
    uint32_t mask = ((uint32_t) 1 << exp) - 1;
    uint32_t step = (hash >> (64 - exp)) | 1;
    
    return (idx + step) & mask;
}

static struct monkey *lookup_monkey(struct htable *htable, const char *name)
{
    uint64_t h = hash(name, strlen(name) + 1);
                      
    for (int32_t i = h;;)
    {
        i = lookup(h, EXP, i);
        if (htable->entries[i] == NULL)
        {
            return NULL;
        }
        else if (strcmp(htable->entries[i]->name, name) == 0)
        {
            return htable->entries[i];
        }
    }
}

static struct monkey *add_monkey(struct htable *htable, struct monkey *monkey)
{
    uint64_t h = hash(monkey->name, strlen(monkey->name) + 1);

    for (int32_t i = h;;)
    {
        i = lookup(h, EXP, i);
        if (htable->entries[i] == NULL)
        {
            if ((uint32_t) htable->num_entries + 1 == (uint32_t) MAX_MONKEYS)
            {
                return NULL;
            }
            htable->num_entries++;
            htable->entries[i] = monkey;
            return htable->entries[i];
        }
        else if (strcmp(htable->entries[i]->name, monkey->name) == 0)
        {
            return htable->entries[i];
        }
    }
}

static long add(long x, long y)
{
    return x + y;
}

static long sub(long x, long y)
{
    return x - y;
}

static long mul(long x, long y)
{
    return x * y;
}

static long divide(long x, long y)
{
    return x / y;
}

static struct htable htable;
static long (*ops[])(long, long) = {
    ['+'] = add, ['-'] = sub, ['*'] = mul, ['/'] = divide
};

static long eval(const char *name)
{
    struct monkey *monkey = lookup_monkey(&htable, name);

    if (monkey->op == '\0')
    {
        return monkey->val;
    }
    else
    {
        return ops[(int) monkey->op](eval(monkey->left), eval(monkey->right));
    }
}

int main(void)
{
    char line[256];
    struct monkey *monkey;

    while (fgets(line, sizeof line, stdin))
    {
        monkey = alloc_monkey();
        if (sscanf(line, "%s %ld\n", monkey->name, &monkey->val) == 2
            || sscanf(line, "%s %s %c %s\n", monkey->name, monkey->left, &monkey->op,
                      monkey->right) == 4)

        {
            monkey->name[strlen(monkey->name) - 1] = '\0';
            add_monkey(&htable, monkey);
        }
    }
    printf("%ld\n", eval("root"));
    return EXIT_SUCCESS;
}
