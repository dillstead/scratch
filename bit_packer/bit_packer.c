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
    
static bool test_pack(uint16_t *input, int *bits, int blen,
                      uint8_t *expect, int elen)
{
    struct bit_packer packer = {0};
    uint8_t output[4] = {0};
    int len = 0;

    for (int b = 0; b < blen; b++)
    {
        len += pack(&packer, input[b], bits[b], output + len);
    }
    len += flush(&packer, output + len);
    return len == elen && memcmp(output, expect, len) == 0;
}

static bool test_unpack(uint8_t *input, int *bits, int blen,
                        uint16_t *expect)
{
    struct bit_packer packer = {0};
    uint16_t output[2] = {0};
    int len = 0;

    for (int b = 0; b < blen; b++)
    {
        len += unpack(&packer, input + len, bits[b], output + b);
    }
    return memcmp(output, expect, blen * sizeof(uint16_t)) == 0;
}

int main(void)
{
    assert(test_pack((uint16_t[]) {0x8001, 0x8001},
                     (int[]) {16, 16}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x01}, 4));
    assert(test_pack((uint16_t[]) {0x8001, 0x1001},
                     (int []) {16, 13}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x08}, 4));
    assert(test_pack((uint16_t[]) {0x8001, 0x401},
                     (int []) {16, 11}, 2,
                     (uint8_t[]) {0x80, 0x01, 0x80, 0x20}, 4));
    assert(test_pack((uint16_t[]) {0x1001, 0x8001, 16},
                     (int []) {13, 16}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x00, 0x08}, 4));
    assert(test_pack((uint16_t[]) {0x1001, 0x1001},
                     (int []) {13, 13}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x00, 0x40}, 4));
    assert(test_pack((uint16_t[]) {0x1001, 0x401},
                     (int []) {13, 11}, 2,
                     (uint8_t[]) {0x80, 0x0C, 0x01}, 3));
    assert(test_pack((uint16_t[]) {0x401, 0x8001},
                     (int []) {11, 16}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x00, 0x20}, 4));
    assert(test_pack((uint16_t[]) {0x401, 0x1001},
                     (int []) {11, 13}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x01}, 3));
    assert(test_pack((uint16_t[]) {0x401, 0x401},
                     (int []) {11, 11}, 2,
                     (uint8_t[]) {0x80, 0x30, 0x04}, 3));

    assert(test_unpack((uint8_t[]) {0x80, 0x01, 0x80, 0x01},
                       (int []) {16, 16}, 2,
                       (uint16_t []) {0x8001, 0x8001}));
    assert(test_unpack((uint8_t[]) {0x80, 0x01, 0x80, 0x08},
                       (int []) {16, 13}, 2,
                       (uint16_t[]) {0x8001, 0x1001}));
    assert(test_unpack((uint8_t[]) {0x80, 0x01, 0x80, 0x20},
                       (int []) {16, 11}, 2,
                       (uint16_t[]) {0x8001, 0x401}));
    assert(test_unpack((uint8_t[]) {0x80, 0x0C, 0x00, 0x08},
                       (int []) {13, 16}, 2,
                       (uint16_t[]) {0x1001, 0x8001, 16}));
    assert(test_unpack((uint8_t[]) {0x80, 0x0C, 0x00, 0x40},
                       (int []) {13, 13}, 2,
                       (uint16_t[]) {0x1001, 0x1001}));
    assert(test_unpack((uint8_t[]) {0x80, 0x0C, 0x01},
                       (int []) {13, 11}, 2,
                       (uint16_t[]) {0x1001, 0x401}));
    assert(test_unpack((uint8_t[]) {0x80, 0x30, 0x00, 0x20},
                       (int []) {11, 16}, 2,
                       (uint16_t[]) {0x401, 0x8001}));
    assert(test_unpack((uint8_t[]) {0x80, 0x30, 0x01},
                       (int []) {11, 13}, 2,
                       (uint16_t[]) {0x401, 0x1001}));
    assert(test_unpack((uint8_t[]) {0x80, 0x30, 0x04},
                       (int []) {11, 11}, 2,
                       (uint16_t[]) {0x401, 0x401}));
    return EXIT_SUCCESS;
}
