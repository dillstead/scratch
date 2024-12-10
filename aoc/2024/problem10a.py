import fileinput

mvs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

def search(x, y):
    cur = grid[x][y]
    if cur == 9:
        ends.add((x, y))
    for mv in mvs:
        if grid[x + mv[0]][y + mv[1]] == cur + 1:
            save = grid[x][y]
            grid[x][y] = -1
            search(x + mv[0], y + mv[1])
            grid[x][y] = save
            
grid = []
heads = []
with fileinput.input() as fin:
    for x, line in enumerate(fin):
        for y, c in enumerate(line):
            if c == '0':
                heads.append((x + 1, y + 1))
        line = line.strip()
        if fin.isfirstline():
            grid.append([-1] * (len(line) + 2))
        grid.append([-1] + [int(x) for x in line] + [-1])
    grid.append([-1] * (len(line) + 2))
sum = 0
for head in heads:
    ends = set()
    search(head[0], head[1])
    sum += len(ends)
print(sum)



