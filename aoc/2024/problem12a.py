import fileinput
from collections import defaultdict
from dataclasses import dataclass

@dataclass 
class Loc:
    x: int
    y: int

    def __hash__(self):
        return hash((self.x, self.y))

mvs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

def cnt_perm(x, y):
    cnt = 0
    for mv in mvs:
        cnt += 1 if grid[x][y] != grid[x + mv[0]][y + mv[1]] else 0
    return cnt

grid = []
nrows = 0
with fileinput.input() as fin:
    for line in fin:
        line = line.strip()
        if fin.isfirstline():
            grid.append('*' * (len(line) + 2))
        grid.append('*' + line + '*')
        nrows += 1
    grid.append('*' * (len(line) + 2))

counts = defaultdict(int)
seen = set()
for x in range(1, nrows + 1):
    for y in range(1, len(line) + 1):
        area, perm = 0, 0
        queue = []
        queue.append(Loc(x, y))
        while queue:
            loc = queue.pop(0)
            if loc in seen:
                continue
            seen.add(loc)
            area += 1
            perm += cnt_perm(loc.x, loc.y)
            for mv in mvs:
                if grid[loc.x + mv[0]][loc.y + mv[1]] == grid[loc.x][loc.y]:
                    queue.append(Loc(loc.x + mv[0], loc.y + mv[1]))
        if area > 0:
            counts[grid[x][y]] += area * perm
sum = 0            
for count in counts.values():
    sum += count
print(sum)

    
