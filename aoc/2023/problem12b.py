import fileinput

def expand(nums):
    group = [' ', '*']
    for num in map(int, nums.split(',')):
        group += num * ['#'] + ['+']
    group[-1] = '*'
    return group

def count(c, m, i, j):
    if i == 0 and j < 2:
        return 1
    if (i == 0 or j == 0 or (c == '.' and group[j] == '#')
        or (c == '#' and group[j] == '+' and m < 1)):
        return 0
    if (c, m, i, j) in memo:
        return memo[(c, m, i, j)]
    if c == '?':
        sum = count('#', m, i, j) + count('.', m, i, j)
    elif c == '#':
        if group[j] == '#':
            sum = count(record[i - 1], 0, i - 1, j - 1)
        else:
            sum = count(record[i], 0, i, j - 1)
    else:
        sum = count(record[i - 1], m + 1, i - 1, j)
    memo[(c, m, i, j)] = sum
    return sum

memo = {}
total = 0
for line in fileinput.input():
    record, nums = line.split()
    record, group = [' '] + list('?'.join([record] * 5)), expand(','.join([nums] * 5))
    total += count(record[-1], 0, len(record) - 1, len(group) - 1)
    memo.clear()
print(total)

