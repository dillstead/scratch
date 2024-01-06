import fileinput
import re

def flood_fill(i, j):
    meters = 0
    stack = [(i, j)]

    while stack:
        i, j = stack.pop()
        if (i < 0 or j < 0 or i >= len(grid) or j >= len(grid[0])
            or grid[i][j] == '#'):
            continue
        meters += 1
        grid[i][j] = '#'
        stack.append((i - 1, j))
        stack.append((i + 1, j))
        stack.append((i, j - 1))
        stack.append((i, j + 1))
    return meters
    
def draw_line(point1, point2):
    meters = 0
    if point1[0] == point2[0]:
        for j in range(min(point1[1], point2[1]), max(point1[1], point2[1]) + 1):
            grid[point1[0]][j] = '#'
            meters += 1
    else:
        for i in range(min(point1[0], point2[0]), max(point1[0], point2[0]) + 1):
            grid[i][point1[1]] = '#'
            meters += 1
    return meters - 1

def get_range(cmds):
    i, j = 0, 0
    mi, mj, xi, xj = 0, 0, 0, 0
    points = []

    points.append((0, 0))
    for cmd in cmds:
        if cmd[0] == 'U':
            i -= cmd[1]
        elif cmd[0] ==  'D':
            i += cmd[1]
        elif cmd[0] == 'L':
            j -= cmd[1]
        else:
            j += cmd[1]
        points.append((i, j))
        mi, mj, xi, xj, = min(i, mi), min(j, mj), max(i, xi), max(j, xj)
    points = list(map(lambda point: (point[0] + abs(mi), point[1] + abs(mj)), points))
    return xi - mi + 1, xj - mi + 1, points

cmds = []
for line in fileinput.input():
    dir, step, color = line.strip().split()
    cmds.append((dir, int(step)))
    
ri, rj, points = get_range(cmds)
pairs = [(points[i], points[i + 1]) for i in range(len(points) - 1)]
grid = [['.' for j in range(rj)] for i in range(ri)]
meters = 0
for pair in pairs:
    meters += draw_line(pair[0], pair[1])

PATTERN = re.compile('#\.+#')
for i, r in enumerate(grid):
    m = PATTERN.search(''.join(r))
    if m is not None:
        meters += flood_fill(i, m.span()[0] + 1)
        break
print(meters)
