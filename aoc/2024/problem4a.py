import fileinput

def search(i, j, xfrm, s, level):
    if level == 4:
        return s == 'XMAS'
    s += grid[i][j]
    return search(i + xfrm[0], j + xfrm[1], xfrm, s, level + 1)

grid = []
with fileinput.input() as fin:
    for line in fin:
        line = line.strip()
        if fin.isfirstline():
            grid.extend(['*' * (len(line) + 6)] * 3)
        grid.append('***' + line + '***')
    grid.extend(['*' * (len(line) + 6)] * 3)

xfrms = [(i, j) for i in range(-1, 2) for j in range(-1, 2)]
sum = 0
for i in range(3, len(grid) - 3):
    for j in range(3, len(grid[i]) - 3):
        for xfrm in xfrms:
            sum += search(i, j, xfrm, "", 0)
print(sum)            
            

