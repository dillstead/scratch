import fileinput
from collections import defaultdict

counts = defaultdict(lambda: 0)
for line in fileinput.input():
    changes = line.strip().split(':')
    counts[changes[1]] += int(changes[0])
best = 0
for val in counts.values():
    if val > best:
        best = val
print(best)

    
    
