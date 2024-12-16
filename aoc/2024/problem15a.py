import fileinput

def push(y, x, m):
    sy = y
    sx = x
    while grid[y][x] == 'O':
        y += mv[m][0]
        x += mv[m][1]
    if grid[y][x] == '.':
        grid[y][x] = 'O'
        grid[sy][sx] = '.'
        return True
    else:
        return False

mv = { '<' : (0, -1), 'v' : (1, 0), '>' : (0, 1), '^' : (-1, 0) }
undo = { '<' : (0, 1), 'v' : (-1, 0), '>' : (0, -1), '^' : (1, 0) }
grid = []
with fileinput.input() as fin:
    for sy, line in enumerate(fin):
        if line.isspace():
            break
        line = line.strip()
        grid.append(list(line))
        sx  = line.find('@')
        if sx != -1:
            y = sy
            x = sx
            grid[y][x] = '.'
    for line in fin:
        line = line.strip()
        for m in line:
            y += mv[m][0]
            x += mv[m][1]
            if grid[y][x] == '#' or (grid[y][x] == 'O' and not push(y, x, m)):
                y += undo[m][0]
                x += undo[m][1]
    sum = 0
    for ty in range(len(grid)):
        for tx in range(len(grid[0])):
            if grid[ty][tx] == 'O':
                sum += 100 * ty + tx
    print(sum)
