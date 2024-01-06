import re
from sys import stdin

def xfrm(seeds, xfrms):
    new_seeds = []
    for seed in seeds:
        for xfrm in xfrms:
            if seed >= xfrm[1] and seed < xfrm[1] + xfrm[2]:
                seed = xfrm[0] + (seed - xfrm[1])
                break
        new_seeds.append(seed)
    return new_seeds

SEEDS = re.compile("(\d+)")
seeds = list(map(int, SEEDS.findall(stdin.readline())))
xfrms = []
while True:
    line = stdin.readline();
    if not line:
        break
    if 'map' in line:
        seeds = xfrm(seeds, xfrms)
        xfrms = []
    if line[0].isdigit():
        xfrms.append(list(map(int, line.strip().split(' '))))
print(min(xfrm(seeds, xfrms)))



    
