// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem10b problem10b.c
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define TARGET         40

#define NOOP_OPCODE    0x0
#define ADD_OPCODE     0x1

#define PACK_OPCODE(x, inst) (((x) << 30) | (inst))
#define UNPACK_OPCODE(inst)  ((inst) >> 30)

unsigned int target = TARGET;
unsigned int cycle = 1;
int32_t reg = 1;

static uint32_t pack_constant(int32_t constant, int width, uint32_t instr)
{
    uint32_t mask = (1 << (width + 1)) - 1;

    return instr | (constant & mask);
}

static int32_t unpack_constant(int width, uint32_t instr)
{
    uint32_t mask = (1 << (width + 1)) - 1;

    return (int32_t) ((instr & mask) << (32 - width)) >> (32 - width);
}

static void execute(uint32_t *instrs, int cnt)
{
    for (int i = 0; i < cnt; i++)
    {
        if (cycle - 1 >= (unsigned int) reg - 1
            && cycle - 1 <= (unsigned int) reg + 1)
        {
            printf("#");
        }
        else
        {
            printf(".");
        }
        if (cycle == target)
        {
            printf("\n");
            cycle = 0;
        }
        cycle++;
        switch (UNPACK_OPCODE(instrs[i]))
        {                    
        case NOOP_OPCODE:
        {
            break;
        }
        case ADD_OPCODE:
        {
            reg += unpack_constant(20, instrs[i]);
            break;
        }
        default:
        {
            puts("Unknown opcode");
            break;
        }
        }
    }
}

int main(void)
{
    char line[256];
    uint32_t instrs[10];
    int cnt;
    int32_t val;

    while (fgets(line, sizeof line, stdin))
    {
        cnt = 0;
        memset(instrs, 0, sizeof instrs);
        if (strcmp(line, "noop") == 0)
        {
            instrs[cnt] = PACK_OPCODE(NOOP_OPCODE, instrs[cnt]);
        }
        else if (sscanf(line, "addx %d", &val) == 1)
        {
            instrs[cnt] = PACK_OPCODE(NOOP_OPCODE, instrs[cnt]);
            cnt++;
            instrs[cnt] = PACK_OPCODE(ADD_OPCODE, instrs[cnt]);
            instrs[cnt] = pack_constant(val, 20, instrs[cnt]);
        }
        cnt++;
        execute(instrs, cnt);
    }
    return EXIT_SUCCESS;
}
