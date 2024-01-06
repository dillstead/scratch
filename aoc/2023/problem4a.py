import re
import fileinput

NUMBERS = re.compile("(\d+)")
sum = 0;

for line in fileinput.input():
    hands = line.split(':')[1].split('|')
    numbers = list(map(int, NUMBERS.findall(hands[0])))x
    winners = list(map(int, NUMBERS.findall(hands[1])))
    match = 0
    for number in numbers:
        if number in winners:
            match += 1
    sum += (int) (2 ** (match - 1))
print(sum)

    



    
