import sys

nums = sys.stdin.read().strip().split()
print(nums)
for i in range(25)):
    news = []
    for num in nums:
        if num == '0':
            news.append('1')
        elif len(num) % 2 == 0:
            news.append(num[0:len(num) // 2])
            news.append(str(int(num[len(num) // 2:])))
        else:
            news.append(str(int(num) * 2024))
    nums = news
print(len(news))


