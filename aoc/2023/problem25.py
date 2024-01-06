import fileinput
from collections import defaultdict

def vertex_id():
    global num_vertices
    num_vertices += 1
    return num_vertices

num_edges = 0
num_vertices = -1
vertices = defaultdict(vertex_id)

with open('problem25_edges.c', 'w') as f:
    f.write('''#include "problem25_edges.h"

struct edge edges[NUM_EDGES] =
{
''')

    for line in fileinput.input():
        vertex, edges = line.strip().split(': ')
        for edge in edges.split(' '):
            f.write(f'{{ {vertices[vertex]}, {vertices[edge]} }},\n')
            num_edges += 1
    num_vertices += 1
        
    f.write('''};
''')

with open('problem25_edges.h', 'w') as f:
    f.write(f'''#ifndef PROBLEM25_EDGES
#define PROBLEM25_EDGES
#define NUM_VERTICES {num_vertices}
#define NUM_EDGES    {num_edges}

struct edge
{{
    int src;
    int dest;
}};

extern struct edge edges[{num_edges}];

#endif
''')
