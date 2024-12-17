import fileinput
import heapq
from collections import namedtuple

N, S, E, W = 1, 2, 3, 4
Node = namedtuple('Node', ['y', 'x', 'dir'])
Entry = namedtuple('Entry', ['min', 'node'])

grid = []
for y, line in enumerate(fileinput.input()):
    line = line.strip()
    grid.append(list(line))
    x = line.find('S')
    if x != -1:
        sy = y
        sx = x
        grid[sy][sx] = '.'
    x = line.find('E')
    if x != -1:
        ey = y
        ex = x
        grid[ey][ex] = '.'
mvs = { N: (-1, 0), S: (1, 0), E: (0, 1), W: (0, -1) }
clock = { N : E, S : W, E : S, W : N }
cclock = { N : W, S : E, E : N, W : S }
heap = []
done = set()
heapq.heappush(heap, Entry(0, Node(sy, sx, E)))
while heap:
    entry = heapq.heappop(heap)
    node = entry.node
    if node in done:
        continue
    done.add(node)
    if node.y == ey and node.x == ex:
        print(entry.min)
        break
    mv = mvs[node.dir]
    if grid[node.y + mv[0]][node.x + mv[1]] != '#':
        heapq.heappush(heap, Entry(entry.min + 1, Node(node.y + mv[0], \
                                                       node.x + mv[1], \
                                                       node.dir)))
    heapq.heappush(heap, Entry(entry.min + 1000, Node(node.y, \
                                                      node.x, \
                                                      clock[node.dir])))
    heapq.heappush(heap, Entry(entry.min + 1000, Node(node.y, \
                                                      node.x, \
                                                      cclock[node.dir])))
