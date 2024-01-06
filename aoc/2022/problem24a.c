// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=address,undefined -g3 -o problem24a problem24a.c
// gcc -Werror -pedantic -std=c99 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -O2 -o problem24a problem24a.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//#define MAX_COL     7
//#define MAX_ROW     5
//#define MAX_MINS    12
#define MAX_COL     121
#define MAX_ROW     26
#define MAX_MINS    600
#define START_COL   1
#define START_ROW   1
#define END_COL     (MAX_COL - 1)
#define END_ROW     (MAX_ROW - 1)
#define MAX_STATES  ((MAX_ROW - 1) * (MAX_COL - 1) * MAX_MINS)
#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))

struct offset
{
    int row_off;
    int col_off;
};

struct state
{
    int row;
    int col;
    int min;
    int total_mins;
};
    
struct queue
{
    int head;
    int tail;
    int num_elems;
    struct state buf[MAX_STATES];
};

static int vert_moves[MAX_COL][MAX_ROW];
static int hor_moves[MAX_ROW][MAX_COL];
static bool visited[MAX_ROW][MAX_COL][MAX_MINS];
static struct queue queue;

static int mod(int x, int y)
{
    int val = x % y;

    return (val >= 0 ? val : y + val);
}

static bool full(struct queue *queue)
{
    return (queue->head + 1) % MAX_STATES == queue->tail;
}

static bool empty(struct queue *queue)
{
    return queue->head == queue->tail;
}

static void push(struct queue *queue, struct state *state)
{
    assert(!full(queue));
    queue->buf[queue->head] = *state;
    queue->head = (queue->head + 1) % MAX_STATES;
    queue->num_elems++;
}

static struct state pop(struct queue *queue)
{
    struct state state = queue->buf[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_STATES;
    queue->num_elems--;
    return state;
}

static bool is_valid_move(int row, int col, int min)
{
    if (row < 1 || row >= MAX_ROW || col < 1 || col >= MAX_COL
        || visited[row][col][min])
    {
        return false;
    }
    for (int i = 1; i < MAX_COL; i++)
    {
        if (hor_moves[row][i]
            && mod(i + hor_moves[row][i] * min - 1, MAX_COL - 1) + 1 == col)
        {
            return false;
        }
    }
    for (int i = 1; i < MAX_ROW; i++)
    {
        if (vert_moves[col][i]
            && mod(i + vert_moves[col][i] * min - 1, MAX_ROW - 1) + 1 == row)
        {
            return false;
        }
    }
    return true;
}

static int find_path(int start_row, int start_col, int end_row, int end_col, int start_min, int *end_min)
{
    struct offset offsets[] =
        {
            {-1, 0}, {1, 0}, {0, 0}, {0, -1}, {0, 1}
        };
    int cur_min = start_min;
    int total_mins = 0;
    int short_mins = MAX_STATES;

    *end_min = start_min;
    do
    {
        cur_min = (cur_min + 1) % MAX_MINS;
        total_mins++;
        if (is_valid_move(start_row, start_col, cur_min))
        {
            memset(&queue, 0, sizeof queue);
            memset(visited, 0, sizeof visited);
            
            push(&queue, &(struct state) {start_row, start_col, cur_min, total_mins});
            visited[start_row][start_col][cur_min] = true;

            while (!empty(&queue))
            {
                struct state state = pop(&queue);
                int next_min = (state.min + 1) % MAX_MINS;
                int next_total_mins = state.total_mins + 1;
        
                if (state.row == end_row && state.col == end_col)
                {
                    if (next_total_mins < short_mins)
                    {
                        short_mins = next_total_mins;
                        *end_min = next_min;
                    }
                    break;
                }
                for (int i = 0; (unsigned int) i < ARRAYLEN(offsets); i++)
                {
                    if (is_valid_move(state.row + offsets[i].row_off, state.col + offsets[i].col_off,
                                      next_min))
                    {
                        push(&queue, &(struct state) {state.row + offsets[i].row_off,
                                    state.col + offsets[i].col_off, next_min, next_total_mins});
                        visited[state.row + offsets[i].row_off][state.col + offsets[i].col_off][next_min] = true;
                    }
                }
            }
        }
    } while (cur_min != start_min);
    return short_mins;
}

int main(void)
{
    char line[1024];
    int i, j;

    i = 0;
    while (fgets(line, sizeof line, stdin))
    {
        fflush(stdout);
        
        j = 0;
        while (line[j])
        {
            if (line[j] == '<')
            {
                hor_moves[i][j] = -1;
            }
            else if (line[j] == '>')
            {
                hor_moves[i][j] = 1;
            }
            else if (line[j] == '^')
            {
                vert_moves[j][i] = -1;
            }
            else if (line[j] == 'v')
            {
                vert_moves[j][i] = 1;
            }
            j++;
        }
        i++;
    }

    int end_min;
    printf("%d\n", find_path(START_ROW, START_COL, END_ROW, END_COL, 0, &end_min);
    return EXIT_SUCCESS;
}
