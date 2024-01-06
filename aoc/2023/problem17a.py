import fileinput
import heapq
from enum import Enum
from collections import namedtuple

Node = namedtuple('Node', ['min', 'i', 'j', 'count', 'dir'])
Key = namedtuple('Key', ['i', 'j', 'count', 'dir'])

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4

def add_node(my_min, key):
    if key.i == 0 or key.j == 0 or key.i == len(grid) - 1 or key.j == len(grid[0]) - 1:
        return
    if key not in heat or heat[key] > my_min + grid[key.i][key.j]:
        heat[key] = my_min + grid[key.i][key.j]
        heapq.heappush(heap, Node(heat[key], key.i, key.j, key.count, key.dir)) 

grid = []
for line in fileinput.input():
    grid.append(list(map(int, list('0' + line.strip() + '0'))))
grid.insert(0, [0] * len(grid[0]))
grid.append([0] * len(grid[0]))

done = set()
heat = {}
heap = []
min_heat = len(grid) * len(grid[0]) * 9
heat[Key(1, 1, 1, Direction.SOUTH.value)] = 0
heat[Key(1, 1, 1, Direction.EAST.value)] = 0
heapq.heappush(heap, Node(0, 1, 1, 1, Direction.SOUTH.value))
heapq.heappush(heap, Node(0, 1, 1, 1, Direction.EAST.value))

while (heap):
    node = heapq.heappop(heap)
    if node in done:
        continue
    done.add((node.i, node.j, node.count, node.dir))
    my_min = heat[Key(node.i, node.j, node.count, node.dir)]
    if node.i == len(grid) - 2 and node.j == len(grid[0]) - 2:
        min_heat = min(my_min, min_heat)
    if node.dir == Direction.NORTH.value or node.dir == Direction.SOUTH.value:
        add_node(my_min, Key(node.i, node.j - 1, 1, Direction.WEST.value))
        add_node(my_min, Key(node.i, node.j + 1, 1, Direction.EAST.value))
        if node.count < 3:
            add_node(my_min, Key(node.i + (1 if node.dir == Direction.SOUTH.value else -1),
                                 node.j, node.count + 1, node.dir))
    if node.dir == Direction.WEST.value or node.dir == Direction.EAST.value:
        add_node(my_min, Key(node.i - 1, node.j, 1, Direction.NORTH.value))
        add_node(my_min, Key(node.i + 1, node.j, 1, Direction.SOUTH.value))
        if node.count < 3:
            add_node(my_min, Key(node.i, node.j + (1 if node.dir == Direction.EAST.value else -1),
                                 node.count + 1, node.dir))
print(min_heat)
