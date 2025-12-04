import fileinput
from itertools import combinations

tot = 0
for line in fileinput.input():
    bats = [int(c) for c in line.strip()]
    m = max(list(combinations(bats, 12)))
    tot += m[0] * 10 + m[1]
print(tot)

    
    
    
    
    
