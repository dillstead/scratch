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
// Adjust this size for a larger table (it must be a power of 2!).
// To avoid linear search for tables that are close to capacity and since this
// table can't expand in size, try and keep it around 50% full or so at maximum
// capacity.
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

// Adds a tag to the table.  Returns true if it was added, false if it's already
// there or the table is at maximum capacity.
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

// Removes a tag from the table.  If the tag isn't in the table, does nothing.
static void binder_tag_remove(struct hash_tbl *ht, uintptr_t handle)
{
    uint32_t idx;
    
    if (lookup(ht, handle, &idx))
    {
        ht->tbl[idx] = tombstone;
        ht->len--;
    }
}

// Retrieves a tag from the table and stores a pointer to it in tag. Returns true
// if the tag was retrieved, false if it's not in the table.
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

    assert(binder_tag_add(&ht, 0, "IActivityManager"));
    assert(binder_tag_add(&ht, 172, "IMessenger"));
    assert(binder_tag_add(&ht, 2245, "IActivityThread"));
    // Table filled, can't add anymore.
    assert(!binder_tag_add(&ht, 2245, "IServiceManager"));
    assert(ht.len == 3);
    assert(binder_tag_get(&ht, 172, &tag));
    assert(tag->handle == 172);
    assert(!strcmp(tag->interface, "IMessenger"));
    assert(!binder_tag_get(&ht, 19, &tag));
    binder_tag_remove(&ht, 172);
    assert(ht.len == 2);
    assert(!binder_tag_get(&ht, 172, &tag));
    binder_tag_remove(&ht, 0);
    binder_tag_remove(&ht, 2245);
    assert(!binder_tag_get(&ht, 0, &tag));
    assert(!binder_tag_get(&ht, 2245, &tag));
    assert(ht.len == 0);
    assert(ht.tbl[0] == tombstone);
    assert(binder_tag_add(&ht, 0, "IActivityManager"));
    assert(binder_tag_add(&ht, 172, "IMessenger"));
    assert(binder_tag_add(&ht, 2245, "IActivityThread"));
    assert(binder_tag_get(&ht, 0, &tag));
    assert(tag->handle == 0);
    assert(!strcmp(tag->interface, "IActivityManager"));
    assert(binder_tag_get(&ht, 172, &tag));
    assert(binder_tag_get(&ht, 2245, &tag));
    assert(tag->handle == 2245);
    assert(!strcmp(tag->interface, "IActivityThread"));

    return EXIT_SUCCESS;
}
