// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion =Wno-error=sign-conversion -fsanitize=address,undefined -g3 -o problem23a problem23a.c
// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -O2 -o problem23a problem23a.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#define NUM_ROUNDS  10
#define EXP         12
#define MAX_ENTRIES (1 << EXP)
#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

enum direction
{
    NORTH, SOUTH = 3, WEST = 6, EAST = 9, NUM_DIRS = 4
};

struct coordinates
{
    int32_t i;
    int32_t j;
};

struct elf
{
    struct coordinates coords;
    struct coordinates new_coords;
};

struct htable
{
    struct elf *entries[MAX_ENTRIES];
    int32_t num_entries;
};

static int mod(int x, int y)
{
    int val = x % y;

    return (val >= 0 ? val : y + val);
}

static bool coords_cmp(int32_t i, int32_t j, struct elf *e)
{
    return i == e->coords.i && j  == e->coords.j;
}

static bool new_coords_cmp(int32_t i, int32_t j, struct elf *e)
{
    return i == e->new_coords.i && j  == e->new_coords.j;
}

static uint64_t hash(int32_t i, int32_t j)
{
    uint64_t h = (uint64_t) i << 32 | (uint32_t) j;

    return (h - 0x8000000080000000) * 1111111111111111111;
}

static int32_t lookup(uint64_t hash, int exp, int32_t idx)
{
    uint32_t mask = ((uint32_t) 1 << exp) - 1;
    uint32_t step = (uint32_t) (hash >> (64 - exp)) | 1;
    
    return (idx + step) & mask;
}

static struct elf *lookup_elf(struct htable *htable, int32_t i, int32_t j,
                              bool (*cmp)(int32_t i, int32_t j, struct elf *))
{
    uint64_t h = hash(i, j);
                      
    for (int32_t k = (int32_t) h;;)
    {
        k = lookup(h, EXP, k);
        if (htable->entries[k] == NULL)
        {
            return NULL;
        }
        else if (cmp(i, j, htable->entries[k]))
        {
            return htable->entries[k];
        }
    }
}

static struct elf *add_elf(struct htable *htable, int32_t i, int32_t j, struct elf *elf,
                           bool (*cmp)(int32_t i, int32_t j, struct elf *))
{
    uint64_t h = hash(i, j);

    for (int32_t k = (int32_t) h;;)
    {
        k = lookup(h, EXP, k);
        if (htable->entries[k] == NULL)
        {
            if ((uint32_t) htable->num_entries + 1 == (uint32_t) MAX_ENTRIES)
            {
                return NULL;
            }
            htable->num_entries++;
            htable->entries[k] = elf;
            return NULL;
        }
        else if (cmp(i, j, htable->entries[k]))
        {
            struct elf *tmp = htable->entries[k];
            htable->entries[k] = elf;
            return tmp;
        }
    }
}

static struct elf elves[MAX_ENTRIES];
static enum direction dir;
static struct htable htable;
static int num_elves;

int main(void)
{
    char line[8192];
    struct coordinates dir_offs[NUM_DIRS * 3] =
    {
        // NSWE
        { -1, 0 }, { -1, -1 }, { -1, 1 },
        { 1, 0 }, { 1, -1 }, { 1, 1 },
        { 0, -1 }, { -1, -1 }, { 1, -1 },
        { 0, 1 }, { -1, 1 }, { 1, 1 }
    };
    int i, j;

    i = 0;
    while (fgets(line, sizeof line, stdin))
    {
        j = 0;
        while (line[j])
        {
            if (line[j] == '#')
            {
                elves[num_elves].coords.i = i;
                elves[num_elves].coords.j = j;
                elves[num_elves].new_coords = elves[num_elves].coords;
                add_elf(&htable, i, j, elves + num_elves, coords_cmp);
                num_elves++;
            }
            j++;
        }
        i++;
    }

    for (int round = 0; round < NUM_ROUNDS; round++)
    {
        for (i = 0; i < num_elves; i++)
        {
            enum direction new_dir = NUM_DIRS;
            int num_dirs = 0;

            for (int j = 0; j < NUM_DIRS; j++)
            {
                int k;
                
                for (k = 0; k < 3; k++)
                {
                    if (lookup_elf(&htable, elves[i].coords.i + dir_offs[dir + k].i,
                                   elves[i].coords.j + dir_offs[dir + k].j, coords_cmp))
                    {
                        break;
                    }
                }
                if (k == 3)
                {
                    if (new_dir == NUM_DIRS)
                    {
                        new_dir = dir;
                    }
                    num_dirs++;
                }
                dir = mod(dir + 3, ARRAYLEN(dir_offs));
            }
            if (num_dirs != 0 && num_dirs != NUM_DIRS)
            {
                elves[i].new_coords.i = elves[i].coords.i + dir_offs[new_dir].i;
                elves[i].new_coords.j = elves[i].coords.j + dir_offs[new_dir].j;
            }
        }
        dir = mod(dir + 3, ARRAYLEN(dir_offs));
                        
        memset(&htable, 0, sizeof htable);
        for (int i = 0; i < num_elves; i++)
        {
            struct elf *elf = add_elf(&htable, elves[i].new_coords.i, elves[i].new_coords.j,
                                      elves + i, new_coords_cmp);
            if (elf)
            {
                elves[i].new_coords = elves[i].coords;
                elf->new_coords = elf->coords;
            }
        }

        memset(&htable, 0, sizeof htable);
        int num_moved = 0;
        for (int i = 0; i < num_elves; i++)
        {
            if (elves[i].coords.i != elves[i].new_coords.i
                || elves[i].coords.j != elves[i].new_coords.j)
            {
                num_moved++;
            }
            elves[i].coords = elves[i].new_coords;
            add_elf(&htable, elves[i].coords.i, elves[i].coords.j, elves + i, coords_cmp);
        }
    }

    int32_t max_i = INT32_MIN, min_i = INT32_MAX;
    int32_t max_j = INT32_MIN, min_j = INT32_MAX;

    for (int i = 0; i < num_elves; i++)
    {
        max_i = MAX(max_i, elves[i].coords.i);
        min_i = MIN(min_i, elves[i].coords.i);
        max_j = MAX(max_j, elves[i].coords.j);
        min_j = MIN(min_j, elves[i].coords.j);
    }
    printf("%" PRId32 "\n", (max_i - min_i + 1) * (max_j - min_j + 1) - num_elves);
    return EXIT_SUCCESS;
}
