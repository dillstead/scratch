import fileinput
from collections import defaultdict

def bk(R, P, X, max, vertices):
    if not P and not X:
        if len(R) > max:
            max = len(R)
            vertices.clear()
            vertices.extend(R)
            return max
    while P:
        v = P.pop()
        max = bk(R.union({v}), P.intersection(graph[v]), X.intersection(graph[v]), \
                 max, vertices)
        X.add(v)
    return max

graph = defaultdict(lambda: set())
for line in fileinput.input():
    edge = line.strip().split('-')
    graph[edge[0]].add(edge[1])
    graph[edge[1]].add(edge[0])
max = 0
vertices = []
bk(set(), set(graph.keys()), set(), 0, vertices)
vertices.sort()
print(','.join(vertices))


