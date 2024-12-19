import fileinput

def check_design(design):
    if not design:
        return True
    if design in npossible:
        return False
    for p in patterns:
        if design.endswith(p):
            if check_design(design[:-len(p)]):
                return True
    npossible.add(design)
    return False
    
with fileinput.input() as fin:
    for line in fin:
        if line.isspace():
            break;
        patterns = [p.strip() for p in line.split(',')]
    npossible = set()
    possible = 0
    for line in fin:
        if check_design(line.strip()):
            possible += 1
print(possible)
