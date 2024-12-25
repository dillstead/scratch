import fileinput
import sys
from itertools import pairwise
import re

dirs_short_seq = {
    '^^': ['A'],
    'AA': ['A'],
    '>>': ['A'],
    'vv': ['A'],
    '<<': ['A'],
    '<v': ['>A'],
    'v<': ['<A'],
    '<>': ['>>A'],
    '><': ['<<A'],
    '<^': ['>^A'],
    '^<': ['v<A'],
    '<A': ['>>^A', '>^>A'],
    'A<': ['v<<A', '<v<A'],
    'v>': ['>A'],
    '>v': ['<A'],
    'v^': ['^A'],
    '^v': ['vA'],
    'vA': ['>^A', '^>A'],
    'Av': ['<vA', 'v<A'],
    '>^': ['<^A', '^<A'],
    '^>': ['v>A', '>vA'],
    '>A': ['^A'],
    'A>': ['vA'],
    '^A': ['>A'],
    'A^': ['<A']
}

keys_short_seq = {
    'AA' : ['A'],
    '0A' : ['>A'],
    '1A' : ['>v>A', '>>vA'],
    '2A' : ['v>A', '>vA'],
    '3A' : ['vA'],
    '4A' : ['v>v>A', 'v>>vA', '>vv>A', '>v>vA', '>>vvA'],
    '5A' : ['vv>A', 'v>vA', '>vvA'],
    '6A' : ['vvA'],
    '7A' : ['vv>v>A', 'vv>>vA', 'v>vv>A', 'v>v>vA', 'v>>vvA', '>vvv>A', '>vv>vA', '>v>vvA', '>>vvvA'],
    '8A' : ['vvv>A', 'vv>vA', 'v>vvA', '>vvvA'],
    '9A' : ['vvvA'],
    'A0' : ['<A'],
    '00' : ['A'],
    '10' : ['>vA'],
    '20' : ['vA'],
    '30' : ['v<A', '<vA'],
    '40' : ['v>vA', '>vvA'],
    '50' : ['vvA'],
    '60' : ['vv<A', 'v<vA', '<vvA'],
    '70' : ['vv>vA', 'v>vvA', '>vvvA'],
    '80' : ['vvvA'],
    '90' : ['vvv<A', 'vv<vA', 'v<vvA', '<vvvA'],
    'A1' : ['^<<A', '<^<A'],
    '01' : ['^<A'],
    '11' : ['A'],
    '21' : ['<A'],
    '31' : ['<<A'],
    '41' : ['vA'],
    '51' : ['<vA', 'v<A'],
    '61' : ['<<vA', '<v<A', 'v<<A'],
    '71' : ['vvA'],
    '81' : ['<vvA', 'v<vA', 'vv<A'],
    '91' : ['<<vvA', '<v<vA', '<vv<A', 'v<<vA', 'v<v<A', 'vv<<A'],
    'A2' : ['^<A', '<^A'],
    '02' : ['^A'],
    '12' : ['>A'],
    '22' : ['A'],
    '32' : ['<A'],
    '42' : ['v>A', '>vA'],
    '52' : ['vA'],
    '62' : ['<vA', 'v<A'],
    '72' : ['vv>A', 'v>vA', '>vvA'],
    '82' : ['vvA'],
    '92' : ['<vvA', 'v<vA', 'vv<A'],
    'A3' : ['^A'],
    '03' : ['>^A', '^>A'],
    '13' : ['>>A'],
    '23' : ['>A'],
    '33' : ['A'],
    '43' : ['v>>A', '>v>A', '>>vA'],
    '53' : ['v>A', '>vA'],
    '63' : ['vA'],
    '73' : ['vv>>A', 'v>v>A', 'v>>vA', '>vv>A', '>v>vA', '>>vvA'],
    '83' : ['vv>A', 'v>vA', '>vvA'],
    '93' : ['vvA'],
    'A4' : ['^^<<A', '^<^<A', '^<<^A', '<^^<A', '<^<^A'],
    '04' : ['^^<A', '^<^A'],
    '14' : ['^A'],
    '24' : ['^<A', '<^A'],
    '34' : ['^<<A', '<^<A', '<<^A'],
    '44' : ['A'],
    '54' : ['<A'],
    '64' : ['<<A'],
    '74' : ['vA'],
    '84' : ['<vA', 'v<A'],
    '94' : ['<<vA', '<v<A', 'v<<A'],
    'A5' : ['^^<A', '^<^A', '<^^A'],
    '05' : ['^^A'],
    '15' : ['^>A', '>^A'],
    '25' : ['^A'],
    '35' : ['^<A', '<^A'],
    '45' : ['>A'],
    '55' : ['A'],
    '65' : ['<A'],
    '75' : ['v>A', '>vA'],
    '85' : ['vA'],
    '95' : ['<vA', 'v<A'],
    'A6' : ['^^A'],
    '06' : ['^^>A', '^>^A', '>^^A'],
    '16' : ['^>>A', '>^>A', '>>^A'],
    '26' : ['^>A', '>^A'],
    '36' : ['^A'],
    '46' : ['>>A'],
    '56' : ['>A'],
    '66' : ['A'],
    '76' : ['v>>A', '>v>A', '>>vA'],
    '86' : ['v>A', '>vA'],
    '96' : ['vA'],
    'A7' : ['^^<<^A', '^^<^<A', '^^^<<A', '^<^<^A', '^<^^<A', '^<<^^A', '<^^<^A', '<^^^<A', '<^<^^A'],
    '07' : ['^^<^A', '^^^<A', '^<^^A'],
    '17' : ['^^A'],
    '27' : ['^<^A', '^^<A', '<^^A'],
    '37' : ['^<<^A', '^<^<A', '^^<<A', '<^<^A', '<^^<A', '<<^^A'],
    '47' : ['^A'],
    '57' : ['<^A', '^<A'],
    '67' : ['<<^A', '<^<A', '^<<A'],
    '77' : ['A'],
    '87' : ['<A'],
    '97' : ['<<A'],
    'A8' : ['^^^<A', '^^<^A', '^<^^A', '<^^^A'],
    '08' : ['^^^A'],
    '18' : ['^^>A', '^>^A', '>^^A'],
    '28' : ['^^A'],
    '38' : ['^^<A', '^<^A', '<^^A'],
    '48' : ['^>A', '>^A'],
    '58' : ['^A'],
    '68' : ['^<A', '<^A'],
    '78' : ['>A'],
    '88' : ['A'],
    '98' : ['<A'],
    'A9' : ['^^^A'],
    '09' : ['^^^>A', '^^>^A', '^>^^A', '>^^^A'],
    '19' : ['^^>>A', '^>^>A', '^>>^A', '>^^>A', '>^>^A', '>>^^A'],
    '29' : ['>^^A', '^^>A', '^>^A'],
    '39' : ['^^A'],
    '49' : ['^>>A', '>^>A', '>>^A'],
    '59' : ['^>A', '>^A'],
    '69' : ['^A'],
    '79' : ['>>A'],
    '89' : ['>A'],
    '99' : ['A'],
}

def short_seq(seq, level):
    if level == max_level:
        return len(seq)
    if (seq, level) in sols:
        return sols[(seq, level)]
    dirs = 'A' + seq
    short = 0
    for dir in pairwise(dirs):
        cur = sys.maxsize
        for mv in dirs_short_seq[dir[0] + dir[1]]:
            cur = min(short_seq(mv, level + 1), cur)
        short += cur
    sols[(seq, level)] = short
    return short

max_level = 26
seqs = []
comp = 0
sols = {}
for line in fileinput.input():
    codes = 'A' + line.strip();
    short = 0
    for code in pairwise(codes):
        cur = sys.maxsize
        for mv in keys_short_seq[code[0] + code[1]]:
            cur = min(short_seq(mv, 1), cur)
        short += cur
    comp += int(re.sub('[^0-9]', '', line)) * short
print(comp)


