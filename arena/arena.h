#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include "types.h"

#define new(a, t, n) (t *)alloc(a, sizeof(t), _Alignof(t), n)
#define sizeof(x)    (ptrdiff_t) sizeof(x)
#define alignof(x)   (ptrdiff_t) _Alignof(x)
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define lengthof(s)  (countof(s) - 1)

struct arena
{
    byte *beg;
    byte *end;
};

struct arena new_arena(size cap);
void *alloc(struct arena *a, size sz, size align, size count);

#endif
