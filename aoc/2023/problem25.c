// Refer to: https://www.geeksforgeeks.org/introduction-and-implementation-of-kargers-algorithm-for-minimum-cut/
// gcc -Werror -std=c99 -Wall -Wextra -O2 -o problem25 problem25_edges.c problem25.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "problem25_edges.h"

struct set
{
    int parent;
    int rank;
};

struct set sets[NUM_VERTICES];

static void swap(struct edge *edge1, struct edge *edge2)
{
    struct edge tmp;

    tmp = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp;
}

static void randomize_edges(struct edge *edges, int num_edges)
{
    for (int i = num_edges - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(edges + i, edges + j);
    }
}

static int find(struct set *sets, int i)
{
    if (sets[i].parent != i)
    {
        sets[i].parent = find(sets, sets[i].parent);
    }
    return sets[i].parent;
}

static void union_sets(struct set *sets, int i, int j)
{
    int iroot = find(sets, i);
    int jroot = find(sets, j);
 
    if (sets[iroot].rank < sets[jroot].rank)
    {
        sets[iroot].parent = jroot;
    }
    else if (sets[iroot].rank > sets[jroot].rank)
    {
        sets[jroot].parent = iroot;
    }
    else
    {
        sets[iroot].parent = jroot;
        sets[jroot].rank++;
    }
}

int main(void)
{
    int vertices;
    int vertex;
    int group1;
    int cut;
    int i;

    srand((unsigned int)time(NULL));
    while (true)
    {
        
        for (i = 0; i < NUM_VERTICES; i++)
        {
            sets[i].parent = i;
        }
        randomize_edges(edges, NUM_EDGES);
        
        i = 0;
        vertices = NUM_VERTICES;
        while (vertices > 2)
        {
            int set1 = find(sets, edges[i].src);
            int set2 = find(sets, edges[i].dest);
            i++;
 
            if (set1 != set2)
            {
                vertices--;
                union_sets(sets, set1, set2);
            }
        }
 
        cut = 0;
        for (int i = 0; i < NUM_EDGES; i++)
        {
            int set1 = find(sets, edges[i].src);
            int set2 = find(sets, edges[i].dest);
            if (set1 != set2)
            {
                cut++;
            }
        }
        vertex = find(sets, 0);
        group1 = 1;
        for (int i = 1; i < NUM_VERTICES; i++)
        {
            if (vertex == find(sets, i))
            {
                group1++;
            }
        }
        if (cut == 3)
        {
            printf("%d\n", group1 * (NUM_VERTICES - group1));
            break;
        }
    }
    return EXIT_SUCCESS;
}


