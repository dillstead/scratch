#!/usr/bin/python
import fileinput

trees = []
visible = 0

def check_rows(r, c):
    for i in r:
        max = -1
        for j in c:
            if trees[i][j] > max:
                visible += 1
                max = trees[i][j]

def check_cols(c, r):
    for j in c:
        max = -1
        for i in r:
            if trees[i][j] > max:
                visible += 1
                max = trees[i][j]

for line in fileinput.input():
    line = line.strip()
    trees.append([int(s) for s in line])
    visible.append([False for s in range(len(line))])

check_rows(range(len(trees)), range(len(trees[0])))
check_rows(range(len(trees)), range(len(trees[0]) - 1, -1, -1))
check_cols(range(len(trees[0])), range(len(trees)))
check_cols(range(len(trees[0])), range(len(trees) - 1, -1, -1))
print visible
