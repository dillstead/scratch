// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem18b problem18b.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define MAX_X         27
#define MAX_Y         27
#define MAX_Z         27
#define NUM_NEIGHBORS 6
#define MAX_POINTS    (NUM_NEIGHBORS * MAX_X * MAX_Y * MAX_Z)
#define NUM_OFFSETS   NUM_NEIGHBORS

struct point
{
    int x;
    int y;
    int z;
};

struct queue
{
    int head;
    int tail;
    struct point buf[MAX_POINTS];
};

static bool empty(struct queue *queue)
{
    return queue->head == queue->tail;
}

static void push(struct queue *queue, struct point point)
{
    int new_head = (queue->head + 1) % MAX_POINTS;
    
    assert(new_head != queue->tail);
    queue->buf[queue->head] = point;
    queue->head = new_head;
}

static struct point pop(struct queue *queue)
{
    struct point point;

    point = queue->buf[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_POINTS;
    return point;
}

static bool grid[MAX_X][MAX_Y][MAX_Z];
static bool visited[MAX_X][MAX_Y][MAX_Z];

int main(void)
{
    int x, y, z;
    struct queue queue;
    struct point point;
    int area = 0;
    struct point offsets[NUM_OFFSETS] =
    {
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
        {  0,  0,  1 },
        {  0,  0, -1 }
    };

    while (scanf("%d,%d,%d\n", &x, &y, &z) == 3)
    {
        grid[x + 1][y + 1][z + 1] = true;
    }
    
    memset(&queue, 0, sizeof queue);
    push(&queue, (struct point) { 0, 0, 0 });
    visited[0][0][0] = true;
    while (!empty(&queue))
    {
        point = pop(&queue);
        for (int i = 0; i < NUM_OFFSETS; i++)
        {
            if (point.x + offsets[i].x < 0 || point.x + offsets[i].x > MAX_X - 1
                || point.y + offsets[i].y < 0 || point.y + offsets[i].y > MAX_Y - 1
                || point.z + offsets[i].z < 0 || point.z + offsets[i].z > MAX_Z - 1
                || visited[point.x + offsets[i].x][point.y + offsets[i].y][point.z + offsets[i].z])
            {
                continue;
            }
            if (!grid[point.x + offsets[i].x][point.y + offsets[i].y][point.z + offsets[i].z])
            {
                push(&queue, (struct point) { point.x + offsets[i].x, point.y + offsets[i].y, point.z + offsets[i].z });
                visited[point.x + offsets[i].x][point.y + offsets[i].y][point.z + offsets[i].z] = true;
            }
            else                    
            {
                area += 1;
            }
        }
    }
    printf("area: %d\n", area);
    return EXIT_SUCCESS;
}
