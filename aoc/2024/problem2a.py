import fileinput

safe = 0
for line in fileinput.input():
    nums = [int(x) for x in line.split()]
    if (all(nums[i] < nums[i + 1]
            and abs(nums[i + 1] - nums[i]) >= 1
            and abs(nums[i + 1] - nums[i]) <= 3
            for i in range(len(nums) - 1))
        or all(nums[i] > nums[i + 1]
            and abs(nums[i + 1] - nums[i]) >= 1
            and abs(nums[i + 1] - nums[i]) <= 3
            for i in range(len(nums) - 1))):
        safe += 1
print(safe)
