import fileinput
from collections import namedtuple

Node = namedtuple('Node', ['y', 'x'])
Entry = namedtuple('Entry', ['steps', 'node'])

dim = 71
br = 1023
mvs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
grid = []
grid.append(['#' for x in range(dim + 2)])
for y in range(dim):
    grid.append(['#'] + ['.' for x in range(dim)] + ['#'])
grid.append(['#' for x in range(dim + 2)])
for y, line in enumerate(fileinput.input()):
    line = line.strip()
    coords = line.split(',')
    grid[int(coords[1]) + 1][int(coords[0]) + 1] = '#'
    if y == br:
        break
ey, ex = dim, dim
queue = []    
seen = set()
queue.append(Entry(0, Node(1, 1)))
while queue:
    entry = queue.pop(0)
    node = entry.node
    if node.y == ey and node.x == ex:
        print(entry.steps)
        break
    if node not in seen:
        for mv in mvs:
            if grid[node.y + mv[0]][node.x + mv[1]] != '#':
                queue.append(Entry(entry.steps + 1,       \
                                   Node(node.y + mv[0],   \
                                        node.x + mv[1]))) 
    seen.add(node)
