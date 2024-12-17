import fileinput
import re

A, B, C = 4, 5, 6
regs = [i for i in range(4)]
with fileinput.input() as fin:
    for i in range(4, 7):
        regs.append(int(re.findall(r'(\d+)', fin.readline())[0]))
    fin.readline()
    prog = list(map(int, re.findall(r'(\d+)', fin.readline())))
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
            output += str(regs[prog[pc + 1]] % 8) + ','
        case 6:
            regs[B] = int(regs[A] / 2 ** regs[prog[pc + 1]])
        case 7:
            regs[C] = int(regs[A] / 2 ** regs[prog[pc + 1]])
    pc += adj
print(output.strip(','))
      

