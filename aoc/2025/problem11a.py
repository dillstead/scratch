import fileinput

v = {}
for line in fileinput.input():
    toks = line.split()
    src = toks[0][:-1]
    v[src] = toks[1:]
cnt = 0
stack = []
stack.append('you')
while stack:
    src = stack.pop()
    if src == 'out':
        cnt += 1
        continue
    for dst in v[src]:
        stack.append(dst)
print(cnt)

    
