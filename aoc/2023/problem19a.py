import re
from sys import stdin
from collections import namedtuple

Record = namedtuple('Record', ['name', 'rules', 'dest'])

RECORD = re.compile('(\w+)\{(.*,)(\w+)\}')
records = {}
line = stdin.readline()
while line != '\n':
    match = RECORD.match(line)
    if match is None:
        break;
    name, rules, dest = match.group(1), match.group(2), match.group(3)
    records[name] = Record(name, [rule.split(':') for rule in rules.split(',')[:-1]], dest)
    line = stdin.readline()

PART = re.compile('\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\}')
sum = 0
while True:
    line = stdin.readline()
    if not line:
        break;
    match = PART.match(line)
    x, m, a, s = int(match.group(1)), int(match.group(2)), int(match.group(3)), int(match.group(4))
#    print(f'x={x},m={m}.a={a},s={s}')
    name = 'in'
    while name != 'A' and name != 'R':
        record = records[name]
#        print(record)
        found = False
        for rule in record.rules:
#            print(rule[0])
            if eval(rule[0]):
                name = rule[1]
                found = True
                break
        if not found:
            name = record.dest
    if name == 'A':
        sum += x + m + a + s
print(sum)

            
            
