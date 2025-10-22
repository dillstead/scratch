#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

int main(void)
{
    for (uint8_t i = 0; i < (1 << 4); i++)
    {
        // Map the lower nibble of i into the MSB of each of the four nibbles in val.
        uint16_t val = ((uint16_t) (i & 0x8) << 12) | ((uint16_t) (i & 0x4) << 9)
            | ((uint16_t) (i & 0x2) << 6) | ((uint16_t) (i & 0x1) << 3);
        // Pack the MSB of each of the four nibbles into the upper nibble of the
        // second byte. 
        assert(i == (uint8_t) (((uint16_t) (val * 0x249)) >> 12));
    }
    return EXIT_SUCCESS;
}
