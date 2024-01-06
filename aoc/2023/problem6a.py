import re
from sys import stdin


NUMBERS = re.compile("(\d+)")
times = list(map(int, NUMBERS.findall(stdin.readline())))
dists = list(map(int, NUMBERS.findall(stdin.readline())))
print(times)
print(dists)
sum = 1

for time, dist in zip(times, dists):
    cdists = [x for x in range(0, time + 1) if (time - x) * x > dist]
    print(cdists)
    sum *= len([x for x in range(0, time + 1) if (time - x) * x > dist])
print(sum)




