import sys
from collections import defaultdict

nums = defaultdict(int)
for num in sys.stdin.read().strip().split():
    nums[num] += 1
for i in range(75):
    news = defaultdict(int)
    for key in nums.keys():
        if key == '0':
            news['1'] += nums[key];
        elif len(key) % 2 == 0:
            news[key[0:len(key) // 2]] += nums[key]
            news[str(int(key[len(key) // 2:]))] += nums[key]
        else:
            news[str(int(key) * 2024)] += nums[key]
    nums = news
print(sum(nums.values()))
