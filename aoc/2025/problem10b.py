import fileinput
from pulp import LpMinimize, LpProblem, LpVariable, lpSum, value

def solve(btns, targets):
    prob = LpProblem("minimize_presses", LpMinimize)
    vars = []
    for i in range(len(btns)):
        vars.append(LpVariable('x' + str(i), lowBound=0, cat='Integer'))
    prob += lpSum(vars)
    for i, target in enumerate(targets):
        terms = []
        for j, btn in enumerate(btns):
            if btn[i] == 1:
                terms.append(vars[j])
        constraint = lpSum(terms) == target
        prob += constraint
    prob.solve()
    if prob.status != 1:
        return 0
    tot = 0
    for i in range(len(btns)):
        tot += int(value(vars[i]))
    return tot

tot = 0
for line in fileinput.input():
    toks = line.split()
    targets = list(map(int, toks[-1][1:-1].split(',')))
    btns = []
    for tok in toks[1:-1]:
        btn = [0] * len(targets)
        for i in list(map(int, tok[1:-1].split(','))):
            btn[i] = 1
        btns.append(btn)
    tot += solve(btns, targets)
print(tot)

