// gcc -Werror -std=c99 -Wall -Wextra -g -O2 -o handle_hash handle_hash.c
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#define MAX_NAME 128
#define EXP      2
#define TBL_SIZE (1 << EXP)

struct binder_tag
{
    uintptr_t handle;
    char interface[MAX_NAME];
};
    
struct hash_tbl
{
    struct binder_tag *tbl[TBL_SIZE];
    int32_t len;
};

static struct binder_tag tags[TBL_SIZE + 1];
static struct binder_tag *tombstone = tags + TBL_SIZE;

static uint64_t hash(uintptr_t handle, uint32_t *mask, uint32_t *step)
{
    uint64_t h = (uint64_t) handle;

    h *= 1111111111111111111;
    *mask = ((uint32_t) 1 << EXP) - 1;
    *step = (uint32_t) (h >> (64 - EXP)) | 1;
    
    return h;
}

static bool lookup(struct hash_tbl *ht, uintptr_t handle, uint32_t *idx)
{
    uint32_t mask;
    uint32_t step;
    uint64_t h = hash(handle, &mask, &step);

    *idx = TBL_SIZE;
    for (uint32_t i = (uint32_t) h;;)
    {
        i = (i + step) & mask;
        if (!ht->tbl[i])
        {
            *idx = (*idx == TBL_SIZE ? i : *idx);
            return false;
        }
        else if (ht->tbl[i] == tombstone)
        {
            *idx = (*idx == TBL_SIZE ? i : *idx);
        }
        else if (ht->tbl[i]->handle == handle)
        {
            *idx = i;
            return true;
        }
    }    
}

static bool binder_tag_add(struct hash_tbl *ht, uintptr_t handle, const char *interface)
{
    uint32_t idx;
    
    if (!lookup(ht, handle, &idx))
    {
        if (ht->len + 1 == TBL_SIZE)
        {
            return false;
        }
        ht->tbl[idx] = tags + idx;
        ht->tbl[idx]->handle = handle;
        strcpy(ht->tbl[idx]->interface, interface);
        ht->len++;
        return true;
    }
    return false;
}

static void binder_tag_remove(struct hash_tbl *ht, uintptr_t handle)
{
    uint32_t idx;
    
    if (lookup(ht, handle, &idx))
    {
        ht->tbl[idx] = tombstone;
        ht->len--;
    }
}

static bool binder_tag_get(struct hash_tbl *ht, uintptr_t handle, struct binder_tag **tag)
{
    uint32_t idx;

    *tag = NULL;
    if (lookup(ht, handle, &idx))
    {
        *tag = ht->tbl[idx];
        return true;
    }
    return false;
}

int main(void)
{
    struct hash_tbl ht = { 0 };
    struct binder_tag *tag;

    assert(binder_tag_add(&ht, 10, "IActivityManager"));
    assert(binder_tag_add(&ht, 172, "IMessenger"));
    assert(binder_tag_add(&ht, 2245, "IActivityThread"));
    assert(ht.len == 3);
    assert(binder_tag_get(&ht, 172, &tag));
    assert(tag->handle == 172);
    assert(!strcmp(tag->interface, "IMessenger"));
    assert(!binder_tag_get(&ht, 19, &tag));
    binder_tag_remove(&ht, 172);
    assert(ht.len == 2);
    assert(!binder_tag_get(&ht, 172, &tag));

    return EXIT_SUCCESS;
}
