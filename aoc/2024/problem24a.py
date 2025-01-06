import fileinput
from collections import defaultdict
from dataclasses import dataclass, field
from typing import Callable

@dataclass 
class Block:
    a: int
    b: int
    c: int
    op: Callable[int, int]
    done: bool = field(default=False)
    
ops = {'AND' : lambda a, b: a & b,
       'OR'  : lambda a, b: a | b,
       'XOR' : lambda a, b: a ^ b }

signals = defaultdict(lambda: -1)
blocks = []
with fileinput.input() as fin:
    for line in fin:
        if line.isspace():
            break
        name, signal = line.strip().split(': ')
        signals[name] = int(signal)
    for line in fin:
        inp, c = line.strip().split(' -> ')
        a, op, b = inp.split(' ')
        blocks.append(Block(a, b, c, ops[op]))
while True:
    prop = False
    for block in blocks:
        a, b = signals[block.a], signals[block.b]
        if not block.done and a != -1 and b != -1:
            signals[block.c] = block.op(a, b)
            block.done = True
            prop = True
    if not prop:
        break
signals = dict(sorted(filter(lambda item: item[0].startswith('z'), signals.items()), reverse=True))
print(int(''.join([str(signals[k]) for k in signals.keys()]), 2))
