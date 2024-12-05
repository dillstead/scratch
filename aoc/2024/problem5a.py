import fileinput

def check(rules, numbers, orders):
    for i, rule in enumerate(rules):
        try:
            if order[rule[0]] > order[rule[1]]:
                return False
        except KeyError:
            pass
    return True

with fileinput.input() as fin:
    sum = 0
    rules = []
    for line in fin:
        if line.isspace():
            break;
        rules.append(tuple(map(int, line.split('|'))))
    for line in fin:
        order = {}
        numbers = list(map(int, line.split(',')))
        for i, number in enumerate(numbers):
            order[number] = i
        if check(rules, numbers, order):
            sum += numbers[int(len(numbers) / 2)]
    print(sum)
    
