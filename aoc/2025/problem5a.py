import fileinput

def merge(rngs):
    mrngs = []
    cur = rngs.pop(0)
    for nxt in rngs:
        if nxt[0] > cur[1] + 1:
            mrngs.append(cur)
            cur = nxt
        else:
            cur = (cur[0], max(cur[1], nxt[1]))
    mrngs.append(cur)
    return mrngs

read_ids = False
rngs = []
tot = 0
for line in fileinput.input():
    if line.isspace():
        rngs = merge(sorted(rngs))
        read_ids = True
        continue
    if read_ids:
        id = int(line.strip())
        tot += int(any(l <= id <= h for l, h in rngs))
    else:
        rngs.append(tuple(map(int, line.strip().split('-'))))
print(tot)
