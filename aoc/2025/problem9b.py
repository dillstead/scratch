import fileinput
from itertools import combinations

def fill(grid):
    queue = []
    seen = set()
    queue.append((1, 1))
    while queue:
        pt = queue.pop(0)
        if pt in seen:
            continue
        seen.add(pt)
        if grid[pt[0]][pt[1]] == '0':
            grid[pt[0]][pt[1]] = '2'
            queue.append((pt[0], pt[1] + 1))
            queue.append((pt[0], pt[1] - 1))
            queue.append((pt[0] + 1, pt[1]))
            queue.append((pt[0] - 1, pt[1]))

def check_rect(pt1, pt2, grid):
    for x in range(min(pt1[0], pt2[0]), max(pt1[0], pt2[0]) + 1):
        if grid[pt1[1]][x] == '2' or grid[pt2[1]][x] == '2':
            return False
    for y in range(min(pt1[1], pt2[1]), max(pt1[1], pt2[1]) + 1):
        if grid[y][pt1[0]] == '2' or grid[y][pt1[0]] == '2':
            return False
    return True

def draw_line(pt1, pt2, grid):
    if pt1[0] == pt2[0]:
        for y in range(min(pt1[1], pt2[1]), \
                       max(pt1[1], pt2[1]) + 1):
            grid[y][pt1[0]] = "1"
    else:
        for x in range(min(pt1[0], pt2[0]), \
                       max(pt1[0], pt2[0]) + 1):
            grid[pt1[1]][x] = "1"

pts = []
for line in fileinput.input():
    pts.append(tuple(map(int, line.strip().split(','))))
xs = sorted(set([pt[0] for pt in pts]))
ys = sorted(set([pt[1] for pt in pts]))

to_xs = {}
to_xs[5] = xs[0]
to_cxs = {}
to_cxs[xs[0]] = 5
mx = 5
pv = 5
for x, d in zip(xs[1:], [b - a for a, b in zip(xs[:-1], xs[1:])]):
      pv += 2 if d > 1 else 1
      to_xs[pv] = x
      to_cxs[x] = pv
      mx = max(mx, pv)

to_ys = {}
to_ys[5] = ys[0]
to_cys = {}
to_cys[ys[0]] = 5
my = 5
pv = 5
for y, d in zip(ys[1:], [b - a for a, b in zip(ys[:-1], ys[1:])]):
      pv += 2 if d > 1 else 1
      to_ys[pv] = y
      to_cys[y] = pv
      my = max(my, pv)

cpts = [(to_cxs[pt[0]], to_cys[pt[1]]) for pt in pts]
grid = []
grid.append(list('2' * (mx + 6)))
for _ in range(my + 4):
    grid.append(list('2' + (mx + 4) * '0' + '2'))
grid.append(list('2' * (mx + 6)))
            
draw_line(cpts[0], cpts[-1], grid)
for i, cpt in enumerate(cpts[:-1]):
    draw_line(cpts[i], cpts[i + 1], grid)
fill(grid)

mx = 0
for prs in list(combinations(cpts, 2)):
    if check_rect(prs[0], prs[1], grid):
        mx = max(mx, (abs(to_xs[prs[0][0]] - to_xs[prs[1][0]]) + 1) \
                 * (abs(to_ys[prs[0][1]] - to_ys[prs[1][1]]) + 1))
print(mx)
