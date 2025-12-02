import fileinput

pos = 50
pw = 0
for line in fileinput.input():
    pos += (-int(line[1:]) if line[0] == 'L' else int(line[1:]))
    pos %= 100
    pw += (pos == 0)
print(pw)
