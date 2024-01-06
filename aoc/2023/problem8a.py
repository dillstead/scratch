import re
from sys import stdin
from itertools import cycle

DIRECTIONS = re.compile("^(\w+)\s=\s\((\w+),\s(\w+)\)")
moves = cycle(list(stdin.readline().strip()))
nodes = {}

stdin.readline()
while True:
    match = DIRECTIONS.match(stdin.readline().strip())
    if not match:
        break
    nodes[match.group(1)] = (match.group(2), match.group(3))
    print(match.group(1) + ' ' + match.group(2) + ' ' + match.group(3))

#print(nodes)
node = nodes['AAA']
steps = 1
for move in moves:
    print(move)
    print(node)
    take = node[0] if move == 'L' else node[1]
    if take == 'ZZZ':
        break
    node = nodes[take]
    steps += 1
print(steps)

    
    
    


