import fileinput
import re
import sys
from collections import defaultdict
from functools import reduce
import operator

width = 101
height = 103
secs = 100
qwidth = int(width / 2)
qheight = int(height / 2)
quads = [(0, 0), (qwidth + 1, 0), (0, qheight + 1), (qwidth + 1, qheight + 1)]
counts = defaultdict(int)
for line in fileinput.input():
    (x, y, vx, vy) = tuple(map(int, re.findall(r'-?\d+', line)))
    x = (x + secs * vx) % width
    y = (y + secs * vy) % height
    for quad in quads:
        if x >= quad[0] and x < quad[0] + qwidth \
           and y >= quad[1] and y < quad[1] + qheight:
            counts[quad] += 1
            break
total = 1
print(reduce(operator.mul, list(counts.values()), 1))
