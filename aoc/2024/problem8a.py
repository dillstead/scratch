import fileinput
import re
from collections import defaultdict
from itertools import combinations

towers = defaultdict(list)
for maxx, line in enumerate(fileinput.input()):
    for m in re.finditer('[a-zA-Z0-9]', line):
        towers[m.group()].append((maxx, m.start()))
maxy = len(line.strip()) - 1

unique = set()
for coords in towers.values():
    for pair in combinations(coords, 2):
        unique.add((pair[0][0] + (pair[0][0] - pair[1][0]), pair[0][1] + (pair[0][1] - pair[1][1])))
        unique.add((pair[1][0] + (pair[1][0] - pair[0][0]), pair[1][1] + (pair[1][1] - pair[0][1])))
num = 0
for coords in unique:
    if coords[0] >= 0 and coords[0] <= maxx and coords[1] >= 0 and coords[1] <= maxy:
        num += 1
print(num)


    

        
        
        
    
