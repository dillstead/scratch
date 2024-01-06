import fileinput
from dataclasses import dataclass
from collections import defaultdict 

@dataclass 
class Vertex:
    edges: list[str]

def add_edge(name, svertex, edge):
    if edge in vertex.edges:
        print('Duplicate edge!')
        return
    vertex.edges.append(edge)
    dvertex = vertices[edge]
    dvertex.edges.append(name)

vertices = defaultdict(lambda: Vertex([]))
for line in fileinput.input():
    name, edges = line.strip().split(': ')
    vertex = vertices[name]
    for edge in edges.split(' '):
        add_edge(name, vertex, edge)
v, e = 0, 0
for name, vertex in vertices.items():
    print(name, len(vertex.edges))
    v += 1
    e += len(vertex.edges)
print(v, e)


    







    
