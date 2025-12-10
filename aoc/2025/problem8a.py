import math
import fileinput
from dataclasses import dataclass
from itertools import combinations

def dist(a, b):
    s = (a.x - b.x)**2 + \
        (a.y - b.y)**2 + \
        (a.z - b.z)**2
    return math.sqrt(s)

@dataclass
class Junc:
    x: int
    y: int
    z: int
    idx: int
    par: int
    sz: int = 1

def find(j):
    if j.idx != j.par:
        j.par = find(juncs[j.par])
        return j.par
    return j.par

def union(ja, jb):
    ra = juncs[find(ja)]
    rb = juncs[find(jb)]
    if ra == rb:
        return
    if ra.sz < rb.sz:
        ra.par = rb.idx
        rb.sz += ra.sz
    else:
        rb.par = ra.idx
        ra.sz += rb.sz

juncs = []
i = 0
for line in fileinput.input():
    toks = line.strip().split(',')
    juncs.append(Junc(x=int(toks[0]), y=int(toks[1]), z=int(toks[2]), \
                      idx=i, par=i))
    i += 1
pairs = sorted(list(combinations(juncs, 2)),
               key=lambda c: (dist(c[0], c[1]),))
for c in pairs[:1000]:
    union(c[0], c[1])
#print(sorted(list(filter(lambda j: j.par == j.idx, juncs)),
#             key=lambda j: (-j.sz,))[:3])
tot = 1
for j in sorted(list(filter(lambda j: j.par == j.idx, juncs)),
                key=lambda j: (-j.sz,))[:3]:
    tot *= j.sz
print(tot)
    

                
