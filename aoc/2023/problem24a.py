import re
import fileinput
from sympy.abc import m, n
from sympy import solve
from itertools import combinations
from collections import namedtuple

Position = namedtuple('Position', ['x', 'y', 'z', 'dx', 'dy', 'dz'])
POSITION = re.compile('(-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+)')

lines = []
num = 0
for line in fileinput.input():
    match = POSITION.match(line)
    x, y, z = int(match.group(1)), int(match.group(2)), int(match.group(3))
    dx, dy, dz = int(match.group(4)), int(match.group(5)), int(match.group(6))
    lines.append(Position(x, y, z, dx, dy, dz))
lines = list(combinations(lines, 2))
for pair in lines:
#    print(pair[0])
#    print(pair[1])
    x1, y1, dx1, dy1 = pair[0].x, pair[0].y, pair[0].dx, pair[0].dy
    x2, y2, dx2, dy2 = pair[1].x, pair[1].y, pair[1].dx, pair[1].dy
    sol = solve([x1 + dx1 * m - x2 - dx2 * n, y1 + dy1 * m - y2 - dy2 * n], [m, n], dict=True)
    if sol and sol[0][m] >= 0 and sol[0][n] >= 0:
        x = x1 + dx1 * sol[0][m]
        y = y1 + dy1 * sol[0][m]
        if x >= 200000000000000 and x <= 400000000000000 and y >= 200000000000000 and y <= 400000000000000:
#        if x >= 7 and x <= 27 and y >= 7 and y <= 27:
            num += 1
print(num)

            
            
    
    
    
    



                 
                

        
