import fileinput
from itertools import groupby

def isvalid(row, nums):
    runs = [len(list(y)) for x, y in groupby(''.join(row)) if x == '#']
    return True if runs == nums else False
    return 0

def count(row, ri, nums):
    if ri == len(row):
        return 1 if isvalid(row, nums) else 0
    if row[ri] == '?':
        row[ri] = '.'
        c = count(row, ri + 1, nums)
        row[ri] = '#'
        c += count(row, ri + 1, nums)
        row[ri] = '?'
        return c;
    else:
        return count(row, ri + 1, nums)

sum = 0    
for line in fileinput.input():
    row, num = line.split()
    row = list(row)
    nums = list(map(int, num.split(',')))
    sum += count(row, 0, nums)
print(str(sum))
