CC = $(ARM_TOOLCHAIN)-gcc
OBJDUMP = $(ARM_TOOLCHAIN)-objdump
OBJCOPY = $(ARM_TOOLCHAIN)-objcopy
CFLAGS =  -Werror -std=c99 -Wall -Wextra -fno-builtin
LDFLAGS = -nostdlib -lgcc

rel_block: rel_block.c
	$(CC) $(CFLAGS) -o rel_block rel_block.c $(LDFLAGS)

clean:
	rm rel_block




