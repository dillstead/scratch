from itertools import groupby
import fileinput
from enum import Enum
 
class Type(Enum):
    HIGH = 1
    ONE = 2
    TWO = 3
    THREE = 4
    FULL = 5
    FOUR = 6
    FIVE = 7

def classify_hand(hand):
    hand = sorted(hand)
    #print(hand)
    runs = [list(y) for x, y in groupby(hand)]
    #print(str(runs))
    if len(runs) == 1:
        return Type.FIVE
    elif len(runs) == 4:
        return Type.ONE
    elif len(runs) == 5:
        return Type.HIGH
    elif len(runs) == 2:
        if len(runs[0]) == 1 or len(runs[0]) == 4:
            return Type.FOUR
        else:
            return Type.FULL
    elif len(runs[0]) == 3 or len(runs[1]) == 3 or len(runs[2]) == 3:
        return Type.THREE
    else:
        return Type.TWO

trans = ''.maketrans('TKA', 'AYZ')
hands = []
for line in fileinput.input():
    hand, bid = line.strip().split(' ')
    print(hand)
    hand = ''.join(hand.translate(trans))
    print(hand)
    hands.append((classify_hand(hand), hand, int(bid)))
hands = sorted(hands, key=lambda hand: (hand[0].value, hand[1]))
print(hands)
sum = 0
for idx, hand in enumerate(hands):
    sum += (idx + 1) * hand[2]
print(sum)




