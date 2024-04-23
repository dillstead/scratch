#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "arena.h"

struct arena new_arena(size_t cap)
{
    struct arena arena = { 0 };
    
    arena.beg = malloc(cap);
    arena.end = arena.beg ? arena.beg + cap : 0;
    return arena;
}

void *alloc(struct arena *a, size_t size, size_t align, size_t count)
{
    size_t pad = (uintptr_t) a->end & (align - 1);
    assert(count < ((uintptr_t) (a->end - a->beg) - pad) / size);
    return memset(a->end -= size * count + pad, 0, size * count);
}
