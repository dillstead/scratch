from itertools import product
from itertools import pairwise
from collections import defaultdict

def print_paths(node, parents, path, paths):
    if not node:
        paths.append(''.join(path))
        return
    path.append(node)
    for parent in parents[node]:
        print_paths(parent, parents, path, paths)
    path.pop()
    
def shortest_paths(start, end, paths):
    queue = []
    seen = set()
    parents = defaultdict(list)
    parents[start].append('')
    queue.append(start)
    while queue:
        key = queue.pop(0)
        if key in seen:
            continue
        if key == end:
            print_paths(end, parents, [], paths)
            break
        seen.add(key)
        for neigh in keys[key]:
            if neigh not in seen:
                parents[neigh].append(key)
                queue.append(neigh)

def to_dir(path):
    d = []
    for p in pairwise(path):
        d.append(dirs[p[0] + p[1]])
    return ''.join(d)

keys = {
    'A' : ['0', '3'],
    '0' : ['A', '2'],
    '1' : ['4', '2'],
    '2' : ['1', '5', '3', '0'],
    '3' : ['A', '2', '6'],
    '4' : ['7', '5', '1'],
    '5' : ['4', '8', '6', '2'],
    '6' : ['5', '9', '3'],
    '7' : ['4', '8'],
    '8' : ['7', '9', '5'],
    '9' : ['8', '6']
}

dirs = {
    'AA' : '',
    '00' : '',
    '11' : '',
    '22' : '',
    '33' : '',
    '44' : '',
    '55' : '',
    '66' : '',
    '77' : '',
    '88' : '',
    '99' : '',
    'A3' : '^',
    '02' : '^',
    '36' : '^',
    '25' : '^',
    '14' : '^',
    '69' : '^',
    '58' : '^',
    '47' : '^',
    '74' : 'v',
    '85' : 'v',
    '96' : 'v',
    '41' : 'v',
    '52' : 'v',
    '63' : 'v',
    '20' : 'v',
    '3A' : 'v',
    'A0' : '<',
    '32' : '<',
    '21' : '<',
    '65' : '<',
    '54' : '<',
    '98' : '<',
    '87' : '<',
    '0A' : '>',
    '23' : '>',
    '56' : '>',
    '89' : '>',
    '12' : '>',
    '45' : '>',
    '78' : '>',
}

print('keys_short_seq = {')
for p in list(product(['A', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'], repeat=2)):
    paths = []
    shortest_paths(p[0], p[1], paths)
    print(f'\t\'{p[1] + p[0]}\' : [', end='')
    for i in range(len(paths)):
        if i < len(paths) - 1:
            print(f'\'{to_dir(paths[i]) + "A"}\', ', end='')
        else:
            print(f'\'{to_dir(paths[i]) + "A"}\'', end='')
    print('],')
print('}')
