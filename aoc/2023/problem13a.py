from sys import stdin
import numpy as np
def reflect_idx(grid):
    for i in range(1, len(grid)):
        if grid[i - 1] == grid[i]:
            spn  = min(i - 1, len(grid) - 1 - i)
            if grid[i + 1: i + 1 + spn] == list(reversed(grid[i - 1 - spn:i - 1])):
                return i
    return 0
    
grid = []
sum = 0
while True:
    line = stdin.readline()
    if not line or line.isspace():
        sum += 100 * reflect_idx(grid)
        grid = np.array(grid).T.tolist()
        sum += reflect_idx(grid)
        grid = []
        if not line: break
    else:
        grid.append(list(line.strip()))
print(sum)
