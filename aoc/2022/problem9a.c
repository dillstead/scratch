// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem9b problem9b.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#define NUM_SEGMENTS 2
#define TAIL         (NUM_SEGMENTS - 1)

enum dir
{
    NO,
    U,
    R,
    D,
    L,
    UR,
    DR,
    DL,
    UL
};

struct offset
{
    int32_t x;
    int32_t y;
};

static enum dir input_dirs[] =
{
    ['U'] = U,
    ['R'] = R,
    ['D'] = D,
    ['L'] = L
};

// Given a direction, how to move.
static struct offset dirs[] =
{
    [NO] = {  0,  0 },
    [U]  = {  1,  0 },
    [R]  = {  0,  1 },
    [D]  = { -1,  0 },
    [L]  = {  0, -1 },
    [UR] = {  1,  1 },
    [DR] = { -1,  1 },
    [DL] = { -1, -1 },
    [UL] = {  1, -1 }
};

// Given the current state and a direction to move, the state to
// transition to.
static int state_trans[9][9] =
{
    { 0, 1, 3, 5, 7, 2, 4, 6, 8 },
    { 1, 1, 2, 0, 8, 1, 3, 7, 1 },
    { 2, 1, 3, 3, 1, 2, 3, 0, 1 },
    { 3, 2, 3, 4, 0, 3, 3, 5, 1 },
    { 4, 3, 3, 5, 5, 3, 4, 5, 0 },
    { 5, 0, 4, 5, 6, 3, 5, 5, 7 },
    { 6, 7, 5, 5, 7, 0, 5, 6, 7 },
    { 7, 8, 0, 6, 7, 1, 5, 7, 7 },
    { 8, 1, 1, 7, 7, 1, 0, 7, 8 },
};

// Given the current state and a direction to move, the direction
// to move the next segment.
static enum dir dir_trans[9][9] =
{
    { NO, NO, NO, NO, NO, NO, NO, NO, NO },
    { NO, U,  NO, NO, NO, UR, NO, NO, UL },
    { NO, UR, UR, NO, NO, UR, R,  NO, U  },
    { NO, NO, R,  NO, NO, UR, DR, NO, NO },
    { NO, NO, DR, DR, NO, R,  DR, D,  NO },
    { NO, NO, NO, D,  NO, NO, DR, DL, NO },
    { NO, NO, NO, DL, DL, NO, D,  DL, L  },
    { NO, NO, NO, NO, L,  NO, NO, DL, UL },
    { NO, UL, NO, NO, UL, U,  NO, L,  UL }
};

static int32_t pack(int32_t x, int32_t y)
{
    return ((x & 0x3FF) << 10) | (y & 0x3FF);
}

static void set_bit(unsigned char *bitmap, int32_t i)
{
    bitmap[i / 8] |= 0x1 << (i % 8);
}

static int test_bit(unsigned char *bitmap, int32_t i)
{
    return (bitmap[i / 8] >> (i % 8)) & 0x1;
}

static enum dir move(int *state, enum dir dir, struct offset *seg)
{
    enum dir next_dir;

    seg->x += dirs[dir].x;
    seg->y += dirs[dir].y;
    next_dir = dir_trans[*state][dir];
    *state = state_trans[*state][dir];
    return next_dir;
}

int main(void)
{
    char input_dir;
    int cnt;
    enum dir dir;
    static unsigned char visited[1 << 17];
    struct offset segs[NUM_SEGMENTS];
    int states[NUM_SEGMENTS];
    int pos = 0;

    memset(visited, 0, sizeof visited);
    memset(segs, 0, sizeof segs);
    memset(states, 0, sizeof states);

    while (scanf("%c%d\n", &input_dir, &cnt) == 2)
    {
        for (int i = 0; i < cnt; i++)
        {
            dir = input_dirs[(int) input_dir];
            for (int j = 0; j < NUM_SEGMENTS; j++)
            {
                dir = move(states + j, dir, segs + j);
            }
            if (!test_bit(visited, pack(segs[TAIL].x, segs[TAIL].y)))
            {
                set_bit(visited, pack(segs[TAIL].x, segs[TAIL].y));
                pos += 1;                
            }
        }
    }
    printf("%d\n", pos);
    return EXIT_SUCCESS;
}
