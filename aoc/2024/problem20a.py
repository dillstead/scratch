import fileinput
import sys
from collections import namedtuple

Node = namedtuple('Node', ['y', 'x'])

mvs = [(1, 0), (-1, 0), (0, 1), (0, -1)]

def mark(snode, mins):
    queue = []    
    seen = set()
    queue.append(snode)
    while queue:
        node = queue.pop(0)
        if node not in seen:
            for mv in mvs:
                nnode = Node(node.y + mv[0], node.x + mv[1])
                if grid[nnode.y][nnode.x] != '#' \
                   and nnode not in seen:
                    queue.append(nnode)
                    mins[nnode.y][nnode.x] = mins[node.y][node.x] + 1
        seen.add(node)

grid = []
with fileinput.input() as fin:
    for y, line in enumerate(fin):
        line = line.strip()
        if fin.isfirstline():
            grid.append('#' * (len(line) + 2))
        grid.append('#' + line + '#')
        x = line.find('S')
        if x != -1:
            sy, sx = y + 1, x + 1
        x = line.find('E')
        if x != -1:
            ey, ex = y + 1, x + 1
    grid.append('#' * (len(line) + 2))

smins = [[sys.maxsize if grid[j][i] == '#' else 0 for i in range(len(grid[0]))] for j in range(len(grid))]
emins = [[sys.maxsize if grid[j][i] == '#' else 0 for i in range(len(grid[0]))] for j in range(len(grid))]

mark(Node(sy, sx), smins)
mark(Node(ey, ex), emins)
target = smins[ey][ex]
jmps = [(2, 0), (-2, 0), (0, 2), (0, -2)]
total = 0
for y in range(2, len(grid) -2):
    for x in range(2, len(grid[0]) - 2):
        for jmp in jmps:
            if target - (smins[y][x] + 2 + emins[y + jmp[0]][x + jmp[1]]) >= 100:
                total += 1
print(total)
