import fileinput
from operator import add
from operator import mul

for line in fileinput.input():
    toks = line.split()
#    print(toks)
    if fileinput.isfirstline():
        prod = list(map(int, toks))
        sm = prod.copy()
    elif toks[0].isdigit():
        cur = list(map(int, toks))
        sm = list(map(add, sm, cur))
        prod = list(map(mul, prod, cur))
    else:
        res = [
            p if op == '*' else s
            for p, s, op in zip(prod, sm, toks)
        ]
        break
print(sum(res))
