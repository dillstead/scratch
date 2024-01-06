import fileinput
from itertools import combinations

grid = []
galaxies = []
row = 0
rowi = []
i = 0
for line in fileinput.input():
    grid.append(list(line.strip()))
    galaxies += [(row, i) for i, x in enumerate(grid[-1]) if x == '#']
    row += 1
    rowi.append(i)
    i += (2 if all(cell == '.' for cell in grid[-1]) else 1)

col = 0
coli = []
for i in range(len(grid[0])):
    coli.append(col)
    col += (2 if all(grid[j][i] == '.' for j in range(len(grid))) else 1)

sum = 0
for (x, y) in combinations(galaxies,2):
    sum += abs(rowi[x[0]] - rowi[y[0]]) + abs(coli[x[1]] - coli[y[1]])
print(str(sum))

                    
    
