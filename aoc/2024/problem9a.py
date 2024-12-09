import sys

disk = []
for id, bl in enumerate(sys.stdin.read().strip()):
    disk.extend(int(bl) * ([-1] if id % 2 == 1 else [int(id / 2)]))
i, j = 0, len(disk) - 1
while i < j:
    if disk[i] == -1 and disk[j] == -1:
        j -= 1
    elif disk[i] != -1 and disk[j] != -1:
        i += 1
    elif disk[i] == -1 and disk[j] != -1:
        disk[i] = disk[j]
        disk[j] = -1
        i += 1
        j -= 1
    else: 
        i += 1
sum = 0
for i in range(j + 1):
    sum += (i * disk[i])
print(sum)    





