import fileinput
import re

def operate(target, total, nums):
    if total > target:
        return False
    if not nums:
        return total == target
    return operate(target, int(str(total) + str(nums[0])), nums[1:])  \
        or operate(target, total * nums[0], nums[1:])                 \
        or operate(target, total + nums[0], nums[1:])

s = 0
for line in fileinput.input():
    nums = list(map(int, re.split(': | ', line)))
    if operate(nums[0], nums[1], nums[2:]):
        s += nums[0]
print(s)
