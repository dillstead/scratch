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
for c in pairs:
    i -= 1 if find(c[0]) != find(c[1]) else 0
    if i == 1:
        print(c[0].x * c[1].x)
        break
    union(c[0], c[1])
