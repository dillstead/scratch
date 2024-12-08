import fileinput
import re
from functools import reduce
from operator import mul

def operate(target, total, nums):
    if total == target:
        return True
    if total > target or not nums:
        return False
    return operate(target, total * nums[0], nums[1:]) \
        or operate(target, total + nums[0], nums[1:])

s = 0
for line in fileinput.input():
    nums = list(map(int, re.split(': | ', line)))
    if operate(nums[0], nums[1], nums[2:]):
        s += nums[0]
print(s)
    

