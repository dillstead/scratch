import re
import fileinput

#12 red cubes, 13 green cubes, and 14 blue cubes
CUBES = re.compile("(\d+)\s(blue|red|green)")
game = 1;
sum = 0;
for line in fileinput.input():
    print(line)
    possible = True
    for draw in line.split(';'):
        for cube in CUBES.findall(draw):
            if ((cube[1] == 'red' and int(cube[0]) > 12) or
                (cube[1] == 'green' and int(cube[0]) > 13) or
                (cube[1] == 'blue' and int(cube[0]) > 14)):
                possible = False
    print(possible)
    if possible:
        sum += game
    game += 1
print(sum)

    
