import fileinput
from dataclasses import dataclass
from enum import Enum

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4
    
@dataclass 
class Cell:
    val: str
    seen: bool

def advance(i, j, dir):
    if ((dir == Direction.NORTH or dir == Direction.SOUTH)
        and grid[i][j].val == '-'):
        return [((i, j - 1), Direction.WEST), ((i, j + 1), Direction.EAST)]
    elif ((dir== Direction.WEST or dir == Direction.EAST)
        and grid[i][j].val == '|'):
        return [((i - 1, j), Direction.NORTH), ((i + 1, j), Direction.SOUTH)]
    elif ((dir == Direction.NORTH and (grid[i][j].val == '|' or grid[i][j].val == '.'))
        or (dir == Direction.WEST and grid[i][j].val == '\\')
        or (dir == Direction.EAST and grid[i][j].val == '/')):        
        return [((i - 1, j), Direction.NORTH)]
    elif ((dir == Direction.SOUTH and (grid[i][j].val == '|' or grid[i][j].val == '.'))
        or (dir == Direction.WEST and grid[i][j].val == '/')
        or (dir == Direction.EAST and grid[i][j].val == '\\')):
        return [((i + 1, j), Direction.SOUTH)]
    elif ((dir == Direction.EAST and (grid[i][j].val == '-' or grid[i][j].val == '.'))
        or (dir == Direction.NORTH and grid[i][j].val == '/')
        or (dir == Direction.SOUTH and grid[i][j].val == '\\')):
        return [((i, j + 1), Direction.EAST)]
    return [((i, j - 1), Direction.WEST)]

grid = []
for line in fileinput.input():
    grid.append([Cell(c, False) for c in line.strip()])

count = 1
queue = []
seen = set()
move = ((0, 0), Direction.EAST)
seen.add(move)
queue.extend(advance(*move[0], move[1]))
while queue:
    move = queue.pop(0)
    if (move[0][0] >= len(grid) or move [0][0] < 0
        or move[0][1] >= len(grid[0]) or move [0][1] < 0
        or move in seen):
        continue
    if not grid[move[0][0]][move[0][1]].seen:
        count += 1
        grid[move[0][0]][move[0][1]].seen = True
    seen.add(move)
    queue.extend(advance(*move[0], move[1]))
print(count)

