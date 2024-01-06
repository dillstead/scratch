#!/usr/bin/python
import fileinput

# same height or taller stop
trees = []

def check_row(i, j, step):
    height = trees[i][j]
#    print "%d, %d, h: %d" % (i, j, height)
    dist = 0
    j += step
    while j >= 0 and j < len(trees[0]):
        dist += 1
#        print "check %d, %d, h: %d" % (i, j, trees[i][j])
        if trees[i][j] >= height:
            break;
        j += step
#    print dist
    return dist

def check_col(i, j, step):
    height = trees[i][j]
    dist = 0
    i += step
    while i >= 0 and i < len(trees):
        dist += 1
        if trees[i][j] >= height:
            break;
        i += step
    return dist

for line in fileinput.input():
    line = line.strip()
    trees.append([int(s) for s in line])

dist = 0
for i in range(1, len(trees) - 1):
    for j in range(1, len(trees[0]) - 1):
        dist = max(dist, check_row(i, j, 1) * check_row(i, j, -1) \
                   * check_col(i, j, 1) * check_col(i, j, -1))
print dist

