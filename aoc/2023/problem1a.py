import re
import fileinput

NUMBERS = re.compile("(1|2|3|4|5|6|7|8|9|one|two|three|four|five|six|seven|eight|nine)")
NUMBERS_MAP = {
    'one': 1,
    'two': 2,
    'three': 3,
    'four': 4,
    'five': 5,
    'six': 6,
    'seven': 7,
    'eight': 8,
    'nine': 9,
    '1': 1,
    '2': 2,
    '3': 3,
    '4': 4,
    '5': 5,
    '6': 6,
    '7': 7,
    '8': 8,
    '9': 9     
}
sum = 0
for line in fileinput.input():
    numbers =  NUMBERS.findall(line)
    print(numbers)
    sum += NUMBERS_MAP[numbers[0]] * 10 + NUMBERS_MAP[numbers[-1]]
print(sum)
