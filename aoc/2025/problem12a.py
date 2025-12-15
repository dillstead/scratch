import fileinput
import re

pat = r'^\d+x\d+:'

szs = [7, 7, 6, 7, 5, 7]
tot = 0
for line in fileinput.input():
    if re.match(pat, line):
        toks = line.split()
        dim = tuple(map(int, toks[0].strip(':').split('x')))
        nums = list(map(int, toks[1:]))
        sm = sum([a * b for a, b in zip(nums, szs)])
        d = dim[0] * dim[1]

        if dim[0] * dim[1] >= sm \
           and int(dim[0] / 3) * int(dim[1] / 3) >= sum(nums):
            tot += 1
print(tot)

    
    
