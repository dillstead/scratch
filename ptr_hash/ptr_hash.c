// gcc -Werror -std=c99 -Wall -Wextra -g -O2 -o ptr_hash ptr_hash.c
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

#define EXP      12
#define TBL_SIZE (1 << EXP)

struct ht
{
    uintptr_t tbl[TBL_SIZE];
    int32_t len;
};

static uintptr_t tombstone = UINTPTR_MAX;

static uint64_t hash(uintptr_t ptr, uint32_t *mask, uint32_t *step)
{
    uint64_t h = (uint64_t) ptr;

    h *= 1111111111111111111;
    *mask = ((uint32_t) 1 << EXP) - 1;
    *step = (uint32_t) (h >> (64 - EXP)) | 1;
    
    return h;
}

static bool ht_lookup(struct ht *ht, uintptr_t ptr, uintptr_t **slot)
{
    uint32_t mask;
    uint32_t step;
    uint64_t h = hash(ptr, &mask, &step);

    *slot = NULL;
    for (uint32_t i = (uint32_t) h;;)
    {
        i = (i + step) & mask;
        if (!ht->tbl[i])
        {
            *slot = *slot ? *slot : &ht->tbl[i];
            return false;
        }
        else if (ht->tbl[i] == tombstone)
        {
            *slot = *slot ? *slot : &ht->tbl[i];
        }
        else if (ht->tbl[i] == ptr)
        {
            *slot = &ht->tbl[i];
            return true;
        }
    }    
}

static bool ht_add(struct ht *ht, uintptr_t ptr)
{
    uintptr_t *slot;
    
    if (!ht_lookup(ht, ptr, &slot))
    {
        if (ht->len + 1 == TBL_SIZE)
        {
            return false;
        }
        *slot = ptr;
        ht->len++;
        return true;
    }
    return false;
}

static void ht_remove(struct ht *ht, uintptr_t ptr)
{
    uintptr_t *slot;
    
    if (ht_lookup(ht, ptr, &slot))
    {
        *slot = tombstone;
        ht->len--;
    }
}

static bool ht_contains(struct ht *ht, uintptr_t ptr)
{
    uintptr_t *slot;
    
    return ht_lookup(ht, ptr, &slot);
}

int main(void)
{
    struct ht ht = { 0 };

    for (uintptr_t i = 1; i < TBL_SIZE; i++)
    {
        assert(ht_add(&ht, i));
        assert(ht_contains(&ht, i));
    }
    assert(ht.len == TBL_SIZE - 1);
    assert(!ht_contains(&ht, TBL_SIZE));
    ht_remove(&ht, TBL_SIZE);
    for (uintptr_t i = 1; i < TBL_SIZE; i++)
    {
        ht_remove(&ht, i);
        assert(!ht_contains(&ht, i));
    }
    assert(ht.len == 0);
    
    return EXIT_SUCCESS;
}
