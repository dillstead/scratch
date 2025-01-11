import fileinput
import re

def rotate(a, pos, val):
    a &= ~(7 << pos)
    a |= val << pos
    return a
    
def run(prog, a):
    A, B, C = 4, 5, 6
    regs = [i for i in range(4)]
    regs.extend([a, 0, 0])
    pc = 0
    output = ''
    while pc < len(prog):
        adj = 2
        match prog[pc]:
            case 0:
                regs[A] = int(regs[A] / 2 ** regs[prog[pc + 1]]); 
            case 1:
                regs[B] = regs[B] ^ prog[pc + 1]
            case 2:
                regs[B] = regs[prog[pc + 1]] % 8
            case 3:
                if regs[A] != 0:
                    pc = prog[pc + 1]
                    adj = 0
            case 4:
                regs[B] = regs[B] ^ regs[C]
            case 5:
                output += str(regs[prog[pc + 1]] % 8)
            case 6:
                regs[B] = int(regs[A] / 2 ** regs[prog[pc + 1]])
            case 7:
                regs[C] = int(regs[A] / 2 ** regs[prog[pc + 1]])
        pc += adj
    return output

def search(prog, pos, a, output):
    if pos == -3:
        print(bin(a))        
        print(a)
        return True
    for i in range(0, 8):
        a = rotate(a, pos, i)
        o = run(prog, a)
        if len(o) == len(output) and o[pos // 3] == output[pos // 3]:
            if (search(prog, pos - 3, a, output)):
                return True
    return False
    
with fileinput.input() as fin:
    for i in range(4, 8):
        fin.readline()
    output = ''.join(re.findall(r'(\d+)', fin.readline()))
    prog = [int(c) for c in output]
search(prog, len(prog) * 3 - 3, 0, output)
