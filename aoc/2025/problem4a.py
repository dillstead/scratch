import fileinput

grid = []
for line in fileinput.input():
    if line.isspace():
        break
    line = line.strip()
    if fileinput.isfirstline():
        grid.append('.' * (len(line) + 2))
    grid.append('.' + line + '.')
grid.append('.' * (len(line) + 2))
    
trans = [(-1, 0), (1, 0), (0, -1), (0, 1), \
         (1, 1), (-1, -1), (-1, 1), (1, -1)]
tot = 0
for i in range(1, len(grid) - 1):
    for j in range(1, len(grid[i]) - 1):
        if grid[i][j] == '.':
            continue
        adj = 0
        for t in trans:
            if grid[i + t[0]][j + t[1]] == '@':
                adj += 1
        if adj < 4:
            tot += 1
print(tot)

        

        
