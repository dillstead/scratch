import fileinput
from sys import setrecursionlimit
from enum import Enum

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4

def walk(i, j, block, steps, max_steps):
    if i == ei and j == ej:
        return max(max_steps, steps)
    if block != Direction.NORTH and grid[i - 1][j] == '.':
        max_steps = walk(i - 1, j, Direction.SOUTH, steps + 1, max_steps)
    if block != Direction.SOUTH and (grid[i + 1][j] == '.' or grid[i + 1][j] == 'v'):
        max_steps = walk(i + 1, j, Direction.NORTH, steps + 1, max_steps)
    if block != Direction.EAST and (grid[i][j + 1] == '.' or grid[i][j + 1] == '>'):
        max_steps = walk(i, j + 1, Direction.WEST, steps + 1, max_steps)
    if block != Direction.WEST and (grid[i][j - 1] == '.' or grid[i][j - 1] == '<'):
        max_steps = walk(i, j - 1, Direction.EAST, steps + 1, max_steps)
    return max_steps

setrecursionlimit(100000)
grid = []
for line in fileinput.input():
    grid.append(list(line.strip()))
si, sj = 0, grid[0].index('.')
ei, ej = len(grid) - 1, grid[-1].index('.')
max_steps = walk(si, sj, Direction.NORTH, 0, 0)
print(max_steps)
