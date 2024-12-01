import fileinput
from functools import reduce

a = []
b = []
for line in fileinput.input():
    nums = [int(x) for x in line.split()]
    a.append(nums[0])
    b.append(nums[1])
a.sort()
b.sort()
print(reduce(lambda sum, nums: sum + abs(nums[0] - nums[1]), zip(a, b), 0))
