import fileinput

grid = []
for line in fileinput.input():
    grid.append(list(line.strip()))
grid.insert(0, list('#' * len(grid[0])))

for j in range(len(grid[0])):
    for i in range(1, len(grid)):
        if grid[i][j] == 'O':
            k = i
            while grid[k - 1][j] == '.':
                grid[k - 1][j], grid[k][j] = grid[k][j], grid[k - 1][j]
                k -= 1
                
sum = 0
for j in range(len(grid[0])):
    for i in range(1, len(grid)):
        if grid[i][j] == 'O':
            sum += len(grid) - i
print(sum)
