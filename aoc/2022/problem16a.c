

// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem16a problem16a.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_VALVES 64
#define NUM_TIME   30
#define PACK(x, y) ((((x) & 0x1F) << 5) | ((y) & 0x1F))

struct queue
{
    int head;
    int tail;
    int buf[NUM_VALVES];
};

struct valve
{
    char name[3];
    int flow;
    int num_tunnels;
    int dist;
    int tunnels[NUM_VALVES];
};

struct next_valve
{
    int vid;
    int new_time;
};

static int max_flow;

// Use 10 bits to pack both x and y coordinates which can range from 0...25.
static int id(char v1, char v2)
{
    static unsigned char map[1 << 10];
    static unsigned char id = 1;

    if (!map[PACK(v1, v2)])
    {
        map[PACK(v1, v2)] = id++;
    }
    return map[PACK(v1, v2)] - 1;
}

static void push(struct queue *queue, int nid)
{
    queue->buf[queue->head] = nid;
    queue->head = (queue->head + 1) % NUM_VALVES;
}

static int pop(struct queue *queue)
{
    int nid;

    nid = queue->buf[queue->tail];
    queue->tail = (queue->tail + 1) % NUM_VALVES;
    return nid;
}

static bool empty(struct queue *queue)
{
    return queue->head == queue->tail;
}

static void add_tunnel(struct valve *valves, int vid, int tid)
{
    valves[vid].tunnels[valves[vid].num_tunnels++] = tid;
}

static void add_valve(struct valve *valves, char v1, char v2, int vid, int flow)
{
    sprintf(valves[vid].name, "%c%c", v1, v2);
    valves[vid].flow = flow;
}

static void calculate_min_dists(struct valve *valves, int min_dists[][NUM_VALVES])
{
    struct queue queue;
    bool visited[NUM_VALVES];
    int vid;

    memset(&queue, 0, sizeof queue);

    for (int i = 0; i < NUM_VALVES; i++)
    {
        memset(visited, 0, sizeof visited);

        push(&queue, i);
        valves[i].dist = 0;
        visited[i] = true;
        while (!empty(&queue))
        {
            vid = pop(&queue);
            for (int j = 0; j < valves[vid].num_tunnels; j++)
            {
                if (!visited[valves[vid].tunnels[j]])
                {
                    push(&queue, valves[vid].tunnels[j]);
                    visited[valves[vid].tunnels[j]] = true;
                    valves[valves[vid].tunnels[j]].dist = valves[vid].dist + 1;
                    min_dists[i][valves[vid].tunnels[j]] = valves[vid].dist + 1;
                }
            }
        }
    }
}

static int get_next_valves(int *valves, int pos, int time, int *flow_valves, int num_flow_valves,
                           int min_dists[][NUM_VALVES], struct next_valve *next_valves)
{
    bool present[NUM_VALVES];
    int new_time;
    int num_nexts = 0;

    memset(present, 0, sizeof present);
    for (int i = 0; i < pos; i++)
    {
        present[valves[i]] = true;
    }
    for (int i = 0; i < num_flow_valves; i++)
    {
        new_time = time - (min_dists[valves[pos - 1]][flow_valves[i]] + 1);
        if (new_time > 0 && !present[flow_valves[i]])
        {
            next_valves[num_nexts].vid = flow_valves[i];
            next_valves[num_nexts++].new_time = new_time;
        }
    }
    return num_nexts;
}

static void calculate_max_flow(struct valve *valves, int *solution, int pos, int flow,
                               int time, int *flow_valves, int num_flow_valves,
                               int min_dists[][NUM_VALVES])
{
    struct next_valve next_valves[NUM_VALVES];
    int num_nexts;
    int new_flow;

    num_nexts = get_next_valves(solution, pos, time, flow_valves, num_flow_valves,
                                min_dists, next_valves);
    if (num_nexts > 0)
    {
        for (int i = 0; i < num_nexts; i++)
        {
            new_flow = flow + next_valves[i].new_time * valves[next_valves[i].vid].flow;
            solution[pos] = next_valves[i].vid;
            calculate_max_flow(valves, solution, pos + 1, new_flow, next_valves[i].new_time,
                               flow_valves, num_flow_valves, min_dists);
        }
    }
    else
    {
        if (flow > max_flow)
        {
            max_flow = flow;
        }
    }
}

int main(void)
{
    char line[256];
    int solution[NUM_VALVES];
    struct valve valves[NUM_VALVES];
    int min_dists[NUM_VALVES][NUM_VALVES];
    int flow_valves[NUM_VALVES];
    int num_flow_valves = 0;
    int flow;
    unsigned char v1, v2;
    unsigned char t1, t2;
    int pos;
    int total;
        
    memset(valves, 0, sizeof valves);
    while (fgets(line, sizeof line, stdin))
    {
        sscanf(line, "Valve %c%c has flow rate=%d; tunnel%n", &v1, &v2, &flow, &total);
        add_valve(valves, v1, v2, id(v1, v2), flow);
        if (line[total] == 's')
        {           
            total += sizeof "s leads to valves" - 1;
            while (sscanf(line + total, " %c%c,%n", &t1, &t2, &pos) == 2)
            {
                add_tunnel(valves, id(v1, v2), id(t1, t2));
                total += pos;
            }
        }
        else
        {
            sscanf(line + total, " leads to valve %c%c", &t1, &t2);
            add_tunnel(valves, id(v1, v2), id(t1, t2));
        }
    }
    
    for (int i = 0; i < NUM_VALVES; i++)
    {
        if (valves[i].flow > 0)
        {
            flow_valves[num_flow_valves++] = i;
        }
    }
    calculate_min_dists(valves, min_dists);
    solution[0] = 0;
    calculate_max_flow(valves, solution, 1, 0, NUM_TIME, flow_valves, num_flow_valves,
        min_dists);
    printf("%d\n", max_flow);
    
    return EXIT_SUCCESS;
}
