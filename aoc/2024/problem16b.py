import fileinput
import heapq
from collections import namedtuple
from collections import defaultdict
from dataclasses import dataclass
import sys

N, S, E, W = 1, 2, 3, 4
Node = namedtuple('Node', ['y', 'x', 'dir'])
Entry = namedtuple('Entry', ['cost', 'pnode', 'node'])
@dataclass 
class Prev:
    cost: int
    nodes: set()

def add_pth_nodes(node):
    pth_nodes.add((node.y, node.x))
    if node.y == sy and node.x == sx:
        return
    for n in done[node].nodes:
        add_pth_nodes(n)

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
done = defaultdict(lambda: Prev(sys.maxsize, set()))
pth_nodes = set()
heapq.heappush(heap, Entry(0, Node(sy, sx, E), Node(sy, sx, E)))
while heap:
    entry = heapq.heappop(heap)
    node = entry.node
    if entry.cost > done[node].cost:
        continue
    prev = done[node]
    prev.cost = entry.cost
    prev.nodes.add(entry.pnode)
    if node.y == ey and node.x == ex:
        add_pth_nodes(node)
        break
    mv = mvs[node.dir]
    if grid[node.y + mv[0]][node.x + mv[1]] != '#':
        heapq.heappush(heap, Entry(entry.cost + 1, node, \
                                   Node(node.y + mv[0],  \
                                        node.x + mv[1],  \
                                        node.dir)))
    heapq.heappush(heap, Entry(entry.cost + 1000, node,  \
                               Node(node.y,              \
                                    node.x,              \
                                    clock[node.dir])))
    heapq.heappush(heap, Entry(entry.cost + 1000, node,  \
                               Node(node.y,              \
                                    node.x,              \
                                    cclock[node.dir])))
print(len(pth_nodes))
