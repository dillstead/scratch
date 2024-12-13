import sys

disk = []
for id, bl in enumerate(sys.stdin.read().strip()):
    disk.extend(int(bl) * ([-1] if id % 2 == 1 else [int(id / 2)]))
i, j = 0, len(disk) - 1
while i < j:
    t = disk[i]
    if disk[i] == -1 and disk[j] != -1:
        disk[i] = disk[j]
    if t != -1 or disk[j] != -1:
        i += 1
    if t == -1:
        j -= 1	
sum = 0
for i in range(j + 1):
    sum += (i * disk[i])
print(sum)    





