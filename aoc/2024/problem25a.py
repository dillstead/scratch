import fileinput
import itertools

locks = []
keys = []
height = 0
cur = None
for line in fileinput.input():
    if fileinput.isfirstline():
        width = len(line.strip())
        cnts = [0 for i in range(width)]
    if line.isspace():
        cur.append(cnts)
        cnts = [0 for i in range(width)]
        cur = None
    else:
        if cur is None:
            cur = locks if line[0] == '#' else keys
            height = 0
        for i, v in enumerate([1 if c == '#' else 0 for c in line.strip()]):
            cnts[i] += v
        height += 1
cur.append(cnts)        
pairs = 0
for key, lock in itertools.product(keys, locks):
    if all(k + l <= height for k, l in zip(key, lock)):
        pairs += 1
print(pairs)
       
            
            
