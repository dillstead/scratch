#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "arena.h"

struct arena new_arena(size cap)
{
    struct arena arena = { 0 };
    
    arena.beg = malloc((size_t) cap);
    arena.end = arena.beg ? arena.beg + cap : 0;
    return arena;
}

void *alloc(struct arena *a, size sz, size align, size count)
{
    size padding = -(iptr) a->beg & (align - 1);
    size available = a->end - a->beg - padding;
    assert(available >= 0 && count <=  available / sz);
    void *p = a->beg + padding;
    a->beg += padding + count * sz;
    return memset(p, 0, (size_t) (count * sz));
}
