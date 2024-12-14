import fileinput
import re
import sys

def search(ax, ay, bx, by, tx, ty):
    if tx < 0 or ty < 0:
        return sys.maxsize
    if tx == 0 and ty == 0:
        return 0
    if (tx, ty) in memo:
        return memo[(tx, ty)]
    m =  min(search(ax, ay, bx, by, tx - ax, ty - ay) + 3, \
               search(ax, ay, bx, by, tx - bx, ty - by) + 1)
    memo[(tx, ty)] = m
    return m

with fileinput.input() as fin:    
    line = fin.readline()
    sum = 0
    while True:
        a = tuple(map(int, re.findall(r'\d+', line)))
        b = tuple(map(int, re.findall(r'\d+', fin.readline())))
        t = tuple(map(int, re.findall(r'\d+', fin.readline())))
        memo = {}
        s = search(a[0], a[1], b[0], b[1], t[0], t[1])
        if s < sys.maxsize:
            sum += s
        fin.readline()
        line = fin.readline()
        if not line:
            break;
    print(sum)


