import fileinput
import re

sum = 0
for line in fileinput.input():
    for vals in re.findall(r"mul\((\d+),(\d+)\)", line):
        sum += int(vals[0]) * int(vals[1])
print(sum)
