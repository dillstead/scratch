import fileinput
from enum import Enum
from sys import setrecursionlimit
from collections import namedtuple

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4

Vertex = namedtuple('Vertex', ['edges'])
Edge = namedtuple('Edge', ['weight', 'dest'])

def has_edge(vertex, id):
    for edge in vertex.edges:
        if edge.dest == id:
            return True
    return False
    
def compress(i, j, block, src_id, steps):
    if grid[i][j].isdigit():
        id = int(grid[i][j])
        if not has_edge(vertices[src_id], id):
            vertices[src_id].edges.append(Edge(steps, id))
            vertices[id].edges.append(Edge(steps, src_id))
        if visited[id] or id == end_id:
            return
        visited[id] = True
        steps = 0
        src_id = id
        
    if block != Direction.NORTH and grid[i - 1][j] != '#':
        compress(i - 1, j, Direction.SOUTH, src_id, steps + 1)
    if block != Direction.SOUTH and grid[i + 1][j] != '#':
        compress(i + 1, j, Direction.NORTH, src_id, steps + 1)
    if block != Direction.EAST and grid[i][j + 1] != '#':
        compress(i, j + 1, Direction.WEST, src_id, steps + 1)
    if block != Direction.WEST and grid[i][j - 1] != '#':
        compress(i, j - 1, Direction.EAST, src_id, steps + 1)

def walk(vertex, steps, max_steps):
    if visited[vertex]:
        return max_steps
    if vertex == end_id:
        return max(max_steps, steps)
    visited[vertex] = True
    for edge in vertices[vertex].edges:
        max_steps = walk(edge.dest, steps + edge.weight, max_steps)
    visited[vertex] = False
    return max_steps
    
setrecursionlimit(100000)
grid = []
for line in fileinput.input():
    grid.append(list(line.strip()))
si, sj = 0, grid[0].index('.')
id = 1
for i in range(1, len(grid) - 1):
    for j in range(1, len(grid[i]) - 1):
        sum = 0 if grid[i - 1][j] == '#' or grid[i - 1][j] == '.' else 1
        sum += 0 if grid[i + 1][j] == '#' or grid[i + 1][j] == '.' else 1
        sum += 0 if grid[i][j - 1] == '#' or grid[i][j - 1] == '.' else 1
        sum += 0 if grid[i][j + 1] == '#' or grid[i][j + 1] == '.' else 1
        if sum >= 3:
            grid[i][j] = str(id)
            id += 1
i, j = len(grid) - 1, grid[len(grid) - 1].index('.')
grid[i][j] = str(id)
end_id = id                                    
vertices = [Vertex([]) for i in range(id + 1)]
visited = [False for i in range(id + 1)]
                                    
visited[0] = True
compress(si, sj, Direction.NORTH, 0, 0)

visited = [False for i in range(id + 1)]
print(f'{walk(0, 0, 0)}')

