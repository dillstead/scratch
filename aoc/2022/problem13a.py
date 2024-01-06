#!/usr/bin/python
import sys
import json

def compare(left, right):
    if type(left) is int and type(right) is int:
        return right - left
    elif type(left) is list and type(right) is list:
        for i in range(min(len(left), len(right))):
            c = compare(left[i], right[i])
            if c != 0:
                return c
        return len(right) - len(left)
    else:
        if type(left) is int:
            return compare([left], right)
        else:
            return compare(left, [right])

sum = 0
packets = [[[2]], [[6]]]
for packet in sys.stdin:
    if packet.isspace():
        continue
    packets.append(json.loads(packet))
packets = sorted(packets, cmp=compare)
print(packets)


    
