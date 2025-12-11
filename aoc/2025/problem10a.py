import sys
import fileinput
import re

def press(state, btn):
    new = list(state)
    for pos in btn:
        new[pos] = '#' if state[pos] == '.' else '.'
    return "".join(new)

sum = 0
for line in fileinput.input():
    toks = line.split()
    target = toks[0][1:-1]
    btns = []
    for btn in toks[1:-1]:
        btns.append(list(map(int, btn[1:-1].split(','))))
    queue = []
    seen = set()
    queue.append(('.' * len(target), 0))
    while queue:
        state = queue.pop(0)
        if state[0] in seen:
            continue
        if state[0] == target:
            sum += state[1]
            break
        seen.add(state[0])
        for btn in btns:
            queue.append((press(state[0], btn), state[1] + 1))
print(sum)
    
    
