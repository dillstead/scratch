import fileinput
from sys import setrecursionlimit

mv = [(-1, 0), (0, 1), (1, 0), (0, -1)]

def walk(pos, dir):
    if grid[pos[0]][pos[1]] == '*':
        return 1
    if grid[pos[0]][pos[1]] == '#':
        return 0
    seen.add(pos)
    for i in range(4):
        if walk((pos[0] + mv[dir][0], pos[1] + mv[dir][1]), dir):
            return 1
        dir = (dir + 1) % 4
    return 0

setrecursionlimit(100000)
grid = []
seen = set()
with fileinput.input() as fin:
    for x, line in enumerate(fin):
        line = line.strip()
        if fin.isfirstline():
            grid.append('*' * (len(line) + 2))
        grid.append('*' + line + '*')
        y  = line.find('^')
        if y != -1:
            pos = (x + 1, y + 1)
    grid.append('*' * (len(line) + 2))
dir = 0
walk(pos, dir)
print(len(seen))


