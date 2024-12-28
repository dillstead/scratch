import fileinput
from collections import defaultdict

vertices = defaultdict(lambda: set())
for line in fileinput.input():
    edge = line.strip().split('-')
    vertices[edge[0]].add(edge[1])
    vertices[edge[1]].add(edge[0])
total = 0
for vertex in vertices.keys():
    for neigh in vertices[vertex].copy():
        shared = vertices[vertex].intersection(vertices[neigh])
        if shared:
            if vertex[0] == 't' or neigh[0] == 't':
                total += len(shared)
            else:
                total += sum(1 for v in shared if v[0] == 't')
        vertices[vertex].remove(neigh)
        vertices[neigh].remove(vertex)
print(total)




    

