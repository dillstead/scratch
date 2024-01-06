from enum import Enum
from sys import stdin
from sys import setrecursionlimit

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4

def walk(row, col, dir, steps):
    if row == srow and col == scol:
        return steps
    if ((dir == Direction.NORTH and grid[row][col] == '|')
        or (dir == Direction.EAST and grid[row][col] == 'F')
        or (dir == Direction.WEST and grid[row][col] == '7')):
        return walk(row + 1, col, Direction.NORTH, steps + 1)
    elif ((dir == Direction.SOUTH and grid[row][col] == '|')
        or (dir == Direction.EAST and grid[row][col] == 'L')
        or (dir == Direction.WEST and grid[row][col] == 'J')):
        return walk(row - 1, col, Direction.SOUTH, steps + 1)
    elif ((dir == Direction.NORTH and grid[row][col] == 'J')
        or (dir == Direction.SOUTH and grid[row][col] == '7')
        or (dir == Direction.EAST and grid[row][col] == '-')):
        return walk(row, col - 1, Direction.EAST, steps + 1)
    elif ((dir == Direction.NORTH and grid[row][col] == 'L')
        or (dir == Direction.SOUTH and grid[row][col] == 'F')
        or (dir == Direction.WEST and grid[row][col] == '-')):
        return walk(row, col + 1, Direction.WEST, steps + 1)
    return 0


setrecursionlimit(100000)
grid = []
line = stdin.readline().strip()
pad = ['.'] * (len(line) + 2)
grid.append(pad)
srow = 1
row = 1
while True:
    pos = line.find('S')
    if pos != -1:
        scol = pos + 1
        srow = row
    grid.append(list('.' + line + '.'))
    line = stdin.readline().strip()
    if not line:
        break
    row += 1
grid.append(pad)
print(str(int((walk(srow + 1, scol, Direction.NORTH, 0) + 1) / 2)))




    




    
    
