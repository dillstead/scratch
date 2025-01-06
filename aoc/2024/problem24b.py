# ~/node_modules/.bin/mmdc -i gates.mmd -o gates.svg
import fileinput
from collections import namedtuple

Gate = namedtuple('Gate', ['in1', 'in2', 'out', 'op'])

gates = []
with fileinput.input() as fin:
    for line in fin:
        if line.isspace():
            break
    for line in fin:
        inp, c = line.strip().split(' -> ')
        a, op, b = inp.split(' ')
        gates.append(Gate(a if a < b else b, b if b > a else a, c, op))
gates = sorted(gates, key=lambda g: (0 if g.in1.startswith('x') else 1, g))
outputs = {}
print('flowchart LR')
for g in gates:
    outputs[g.out] = g.in1 + g.in2 + g.op
for g in gates:
    if g.in1.startswith('x'):
        print(f'{g.in1}(({g.in1})) --> {g.in1}{g.in2}{g.op}[{g.op}]')
        print(f'{g.in2}(({g.in2})) --> {g.in1}{g.in2}{g.op}[{g.op}]')
    else:
        print(f'{outputs[g.in1]} --> |{g.in1}|{g.in1}{g.in2}{g.op}[{g.op}]')
        print(f'{outputs[g.in2]} --> |{g.in2}|{g.in1}{g.in2}{g.op}[{g.op}]')
    if g.out.startswith('z'):
        print(f'{g.in1}{g.in2}{g.op} --> {g.out}(({g.out}))')
