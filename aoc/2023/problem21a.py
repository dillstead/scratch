import fileinput
from collections import deque
from collections import namedtuple

Node = namedtuple('Node', ['i', 'j', 'step'])

grid = []

for line in fileinput.input():
    grid.append(list('#' + line.strip() + '#'))
grid.insert(0, list('#' * len(grid[0])))
grid.append(list('#' * len(grid[0])))

si, sj = 0, 0
for i, row in enumerate(grid):
    sj = ''.join(row).find('S')
    if sj != -1:
        si = i
        break;

max_steps = 10000
num_gardens = 0
queue = deque()
seen = set()
start = Node(si, sj, 0)
queue.append(start)
while queue:
    node = queue.popleft()
    if node.step > max_steps:
        break
    if node.step == max_steps:
        num_gardens += 1
    i, j, step  = node.i, node.j, node.step + 1
    if grid[i - 1][j] != '#':
        node = Node(i - 1, j, step)
        if node not in seen:
            seen.add(node)
            queue.append(node)
    if grid[i + 1][j] != '#':
        node = Node(i + 1, j, step)
        if node not in seen:
            seen.add(node)
            queue.append(node)
    if grid[i][j - 1] != '#':
        node = Node(i, j - 1, step)
        if node not in seen:
            seen.add(node)
            queue.append(node)
    if grid[i][j + 1] != '#':
        node = Node(i, j + 1, step)
        if node not in seen:
            seen.add(node)
            queue.append(node)
print(num_gardens)
