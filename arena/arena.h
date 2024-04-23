#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

#define sizeof(x)    (ptrdiff_t) sizeof(x)
#define alignof(x)   (ptrdiff_t) _Alignof(x)
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define lengthof(s)  (countof(s) - 1)
#define new(a, t, n) (t *)alloc(a, sizeof(t), _Alignof(t), n)

struct arena
{
    unsigned char *beg;
    unsigned char *end;
};

struct arena new_arena(size_t cap);
void *alloc(struct arena *a, size_t size, size_t align, size_t count);

#endif
