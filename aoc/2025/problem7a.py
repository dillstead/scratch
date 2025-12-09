import fileinput

split = 0
for line in fileinput.input():
    if fileinput.isfirstline():
        prev = '.' + line.replace('S', '|').strip() + '.'
        continue
    cur = list('.' + line.strip() + '.')
    for i in range(1, len(prev) - 1):
        if prev[i] == '|':
            if cur[i] == '.':
                cur[i] = '|'
            elif cur[i] == '^':
                split += 1
                cur[i - 1] = '|'
                cur[i + 1] = '|'
    prev = cur
print(split)
