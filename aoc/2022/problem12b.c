// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem12b problem12b.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_ROWS 41
#define NUM_COLS 171

struct coord
{
    int x;
    int y;
    int step;
    char level;
};

struct queue
{
    int head;
    int tail;
    struct coord buffer[NUM_ROWS * NUM_COLS];
};

char map[NUM_ROWS][NUM_COLS];
struct queue queue;

static bool empty(struct queue *queue)
{
    return queue->head == queue->tail;
}

static void push(struct queue *queue, int x, int y, int step, char level)
{
    queue->buffer[queue->head] = (struct coord) { x, y, step, level };
    queue->head = (queue->head + 1) % (sizeof queue->buffer / sizeof queue->buffer[0]);
}

static void pop(struct queue *queue, int *x, int *y, int *step, char *level)
{
    *x = queue->buffer[queue->tail].x;
    *y = queue->buffer[queue->tail].y;
    *step = queue->buffer[queue->tail].step;
    *level = queue->buffer[queue->tail].level;
    queue->tail = (queue->tail + 1) % (sizeof queue->buffer / sizeof queue->buffer[0]);
}

static void push_neighbor(struct queue *queue, int x, int y, int step, char level)
{
    if (x >= 0 && x < NUM_ROWS && y >= 0 && y < NUM_COLS
        && map[x][y] != '\0' &&  map[x][y] >= level - 1)
    {
        push(queue, x, y, step + 1, map[x][y]);
        map[x][y] = '\0';
    }
}

int main(void)
{
    char line[256];
    int ex, x;
    int ey, y;
    int step;
    int min_step = NUM_ROWS * NUM_COLS;
    char *s;
    char level;
    int i = 0;

    while (fgets(line, sizeof line, stdin))
    {
        s = strchr(line, 'S');
        if (s != NULL)
        {
            *s = 'a';
        }
        s = strchr(line, 'E');
        if (s != NULL)
        {
            *s = 'z';
            ex = i;
            ey = s - line;
        }
        memcpy(map[i], line, strlen(line) - 1);
        i++;
    }
    push(&queue, ex, ey, 0, map[ex][ey]);
    map[ex][ey] = '\0';

    while (!empty(&queue))
    {
        pop(&queue, &x, &y, &step, &level);
        if (level == 'a' && step < min_step)
        {
            min_step = step;
        }
        push_neighbor(&queue, x + 1, y, step, level);
        push_neighbor(&queue, x - 1, y, step, level);
        push_neighbor(&queue, x, y + 1, step, level);
        push_neighbor(&queue, x, y - 1, step, level);
    }
    printf("%d\n", min_step);
    return EXIT_SUCCESS;
}
