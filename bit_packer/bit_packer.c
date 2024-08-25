// gcc -Werror -std=c99 -Wall -Wextra -g3 -o bit_packer bit_packer.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct bit_packer
{
    int used;
    uint8_t byte;
};

struct index_bits
{
    uint16_t index;
    int bits;
};

int pack(struct bit_packer *packer, uint16_t input, int bits, uint8_t *output)
{
    assert(bits > 8 && bits <= 16);
    assert(packer->used >= 0 && packer->used <= 7);

    int pos = 0;

    bits = bits - 8 + packer->used;
    output[pos++] = (uint8_t) (input >> bits) | packer->byte;
    if (bits >= 8)
    {
        bits -= 8;
        output[pos++] = (uint8_t) (input >> bits);
    }
    // Remainder 
    packer->byte = (uint8_t) (input << (8 - bits));
    packer->used = bits;
    return pos;
}

int flush(struct bit_packer *packer, uint8_t *output)
{
    assert(packer->used >= 0 && packer->used <= 7);
        
    int pos = 0;
    
    if (packer->used > 0)
    {
        output[pos++] = packer->byte;
    }
    packer->used = 0;
    packer->byte = 0;
    return pos;
}

int unpack(struct bit_packer *packer, uint8_t *input, int bits, uint16_t *output)
{
    assert(bits > 8 && bits <= 16);
    assert(packer->used >= 0 && packer->used <= 7);

    int pos = 0;
    
    *output = ((uint16_t) packer->byte) << (bits - 8);
    bits -= packer->used;
    packer->byte = input[pos++];
    if (bits > 8)
    {
        *output |= ((uint16_t) packer->byte) << (bits - 8);
        bits -= 8;        
        packer->byte = input[pos++];
    }
    packer->used = 8 - bits;
    *output |= ((uint16_t) packer->byte) >> packer->used;
    packer->byte = (uint8_t) (((uint16_t) packer->byte) << bits);
    return pos;
}
    
static bool test_pack(struct index_bits *input, int ilen,
                      uint8_t *expect, int elen)
{
    struct bit_packer packer = {0};
    uint8_t output[128];
    int e = 0;

    for (int i = 0; i < ilen; i++)
    {
        e += pack(&packer, input[i].index, input[i].bits, output + e);
    }
    e += flush(&packer, output + e);
    return e == elen && memcmp(output, expect, e) == 0;
}

int main(void)
{
    assert(test_pack((struct index_bits[]) {{0x8001, 16}, {0x8001, 16}}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x01}, 4));
    assert(test_pack((struct index_bits[]) {{0x8001, 16},  {0x1001, 13}}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x08}, 4));
    assert(test_pack((struct index_bits[]) {{0x8001, 16}, {0x401, 11}}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x20}, 4));
    assert(test_pack((struct index_bits[]) {{0x1001, 13}, {0x8001, 16}}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x00, 0x08}, 4));
    assert(test_pack((struct index_bits[]) {{0x1001, 13},  {0x1001, 13}}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x00, 0x40}, 4));
    assert(test_pack((struct index_bits[]) {{0x1001, 13}, {0x401, 11}}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x01}, 3));
    assert(test_pack((struct index_bits[]) {{0x401, 11}, {0x8001, 16}}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x00, 0x20}, 4));
    assert(test_pack((struct index_bits[]) {{0x401, 11},  {0x1001, 13}}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x01}, 3));
    assert(test_pack((struct index_bits[]) {{0x401,11}, {0x401, 11}}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x04}, 3));
    return EXIT_SUCCESS;
}
