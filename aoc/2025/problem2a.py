import sys

def has_dups(n, ns):
    mid = len(ns) // 2
    return len(ns) % 2 == 0 \
        and ns[:mid] == ns[mid:]

tot = 0
for rng in sys.stdin.readline().strip().split(','):
    r = rng.split('-')
    for n in range(int(r[0]), int(r[1]) + 1):
        if (has_dups(n, str(n))):
            tot += n
print(tot)
            
        
    
    
