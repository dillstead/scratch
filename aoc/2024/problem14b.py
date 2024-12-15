import fileinput
import re
import sys
from collections import defaultdict
from functools import reduce
import operator

# (aoc) ~/projects/scratch/aoc/2024 (main*) $ python3 ./problem14b.py  < problem14.input | grep xxxxxx
width = 101
height = 103
vecs = []
for line in fileinput.input():
    vecs.append(tuple(map(int, re.findall(r'-?\d+', line))))
scale = 1
secs = 1
while True:
    counts = defaultdict(int)
    grid = [[' ' for x in range(width)] for y in range(height)]
    for vec in vecs:
        x = (vec[0] + scale * vec[2]) % width
        y = (vec[1] + scale * vec[3]) % height
        grid[y][x] = 'x'
    for y in range(len(grid)):
        print(f'secs: {secs}: ', end='')
        for x in range(len(grid[0])):
            print(grid[y][x], end='')
        print('')
    scale += 1
    secs += 1
