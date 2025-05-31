// gcc -Werror -std=gnu99 -Wall -Wextra -fno-builtin -nostdlib -O2 -fsanitize=address,undefined -o s8cpcat s8cpcat
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

struct arena
{
    unsigned char *beg;
    unsigned char *end;
    ptrdiff_t sz;
};

static bool new_arena(struct arena *a, ptrdiff_t sz)
{
    if (sz <= 0)
    {
        return false;
    }
    unsigned char *beg = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
                              -1, 0);
    if (beg == MAP_FAILED)
    {
        perror("mmap failed");
        return false;
    }
    a->sz = sz;
    a->beg = beg;
    a->end = a->beg + a->sz;
    return true;
}

#define new(a, t, n) (t *) alloc(a, sizeof(t), _Alignof(t), n)
static void *alloc(struct arena *a, ptrdiff_t sz, ptrdiff_t align, ptrdiff_t count)
{
    ptrdiff_t pad = -(intptr_t) a->beg & (align - 1);
    ptrdiff_t avail = a->end - a->beg - pad;
    if (avail < 0 || count > avail / sz)
    {
        return NULL;
    }
    unsigned char *p = a->beg + pad;
    a->beg += pad + count * sz;
    return memset(p, 0, (size_t) (count * sz));
}

struct s8
{
    unsigned char *data;
    ptrdiff_t len;
};

#define countof(a)  (ptrdiff_t)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)
#define s8(s)       (struct s8){(unsigned char *)s, lengthof(s)}

static bool s8cpy(struct arena *a, struct s8 *dst, struct s8 src)
{
    dst->len = src.len;
    dst->data = new(a, unsigned char, src.len);
    if (!dst->data)
    {
        return false;
    }
    if (src.data)
    {
        memcpy(dst->data, src.data, (size_t) src.len);
    }
    return true;
}

static bool s8cat(struct arena *a, struct s8 *dst, struct s8 head, struct s8 tail)
{
    *dst = head;
    if ((!head.data || head.data + head.len != a->beg)
        && !s8cpy(a, dst, head))
    {
        return false;
    }
    if (!s8cpy(a, &head, tail))
    {
        return false;
    }
    dst->len += head.len;
    return true;
}

static bool s8equals(struct s8 s1, struct s8 s2)
{
    return s1.len == s2.len
        && (!s1.len || !memcmp(s1.data, s2.data, (size_t) s1.len));
}

int main(void)
{
    struct s8 res;
    struct arena a;

    assert(new_arena(&a, 1 << 12));
    assert(s8cpy(&a, &res, s8("")));
    assert(s8equals(res, s8("")));
    assert(s8cpy(&a, &res, s8("Hello World")));
    assert(s8equals(res, s8("Hello World")));

    assert(s8cat(&a, &res, s8(""), s8("")));
    assert(s8equals(res, s8("")));
    assert(s8cat(&a, &res, s8("Hello"), s8(" World")));
    assert(s8equals(res, s8("Hello World")));
    assert(s8cat(&a, &res, s8("Hello"), s8("")));
    assert(s8equals(res, s8("Hello")));
    assert(s8cat(&a, &res, s8(""), s8(" World")));
    assert(s8equals(res, s8(" World")));

    struct s8 s1;
    struct s8 s2;
    assert(s8cpy(&a, &s1, s8("Hello")));
    assert(s8cpy(&a, &s2, s8(" World")));
    assert(s8cat(&a, &res, s1, s2));
    assert(s8equals(res, s8("Hello World")));
    assert(s8cat(&a, &res, res, s8(" World")));
    assert(s8equals(res, s8("Hello World World")));
    s8cpy(&a, &res, (struct s8) {0});
    assert(res.len == 0);
    s8cat(&a, &res, (struct s8) {0}, (struct s8) {0});
    assert(res.len == 0);
    return EXIT_SUCCESS;
}
