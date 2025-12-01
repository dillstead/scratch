import fileinput

pos = 50
pw = 0
for line in fileinput.input():
    line = line.strip()
    if line[0] == 'L':
        pos = (pos - int(line[1:])) % 100
    else:
        pos = (pos + int(line[1:])) % 100
    pw += (pos == 0)
print(pw)
