// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem12a problem12a.c
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

static void push(struct queue *queue, int x, int y, int step)
{
    queue->buffer[queue->head] = (struct coord) { x, y, step };
    queue->head = (queue->head + 1) % (sizeof queue->buffer / sizeof queue->buffer[0]);
}

static void pop(struct queue *queue, int *x, int *y, int *step)
{
    *x = queue->buffer[queue->tail].x;
    *y = queue->buffer[queue->tail].y;
    *step = queue->buffer[queue->tail].step;
    queue->tail = (queue->tail + 1) % (sizeof queue->buffer / sizeof queue->buffer[0]);
}

static void push_neighbor(struct queue *queue, int x, int y, char level, int step)
{
    if (x >= 0 && x < NUM_ROWS && y >= 0 && y < NUM_COLS && map[x][y] <= level + 1)
    {
        push(queue, x, y, step + 1);
    }
}

int main(void)
{
    char line[256];
    int sx, ex, x;
    int sy, ey, y;
    int step;
    char *s;
    char level;
    int i = 0;

    while (fgets(line, sizeof line, stdin))
    {
        s = strchr(line, 'S');
        if (s != NULL)
        {
            *s = 'a';
            sx = i;
            sy = s - line;
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
    push(&queue, sx, sy, 0);

    while (!empty(&queue))
    {
        pop(&queue, &x, &y, &step);
        if (map[x][y] == '\0')
        {
            continue;
        }
        if (x == ex && y == ey)
        {
            printf("%d\n", step);
            break;
        }
        level = map[x][y];
        map[x][y] = '\0';
        push_neighbor(&queue, x + 1, y, level, step);
        push_neighbor(&queue, x - 1, y, level, step);
        push_neighbor(&queue, x, y + 1, level, step);
        push_neighbor(&queue, x, y - 1, level, step);
    }
    return EXIT_SUCCESS;
}
