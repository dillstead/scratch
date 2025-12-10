import fileinput
from itertools import combinations

pts = []
for line in fileinput.input():
    pts.append(tuple(map(int, line.strip().split(','))))
mx = 0
for prs in list(combinations(pts, 2)):
    mx = max(mx, (abs(prs[0][0] - prs[1][0]) + 1) \
             * (abs(prs[0][1] - prs[1][1]) + 1))
print(mx)

    
