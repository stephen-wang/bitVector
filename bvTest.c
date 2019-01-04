#include <stdio.h>
#include <inttypes.h>

// Set/clear bit
#define BV_BIT_MASK(bit)        (uint8_t)(0x01<<bit)
#define BV_SET_BIT(byte, bit)   byte |= BV_BIT_MASK(bit)
#define BV_CLEAR_BIT(byte, bit) byte &= ~BV_BIT_MASK(bit)

// Get the value of sepecific bit
#define BV_BIT_VAL(byte, bit)   (uint8_t)((byte >> bit) & 0x01)


void checkResult(uint8_t bit, uint8_t expect, uint8_t actual)
{
    if (expect != actual) {
        printf("Failure on bit %u: expect %u, actual %u\n", bit, expect, actual);
    }
}

int main(int argc, char **argv)
{
    uint8_t digit = 0x15;  // b'00010101

    uint8_t bit;
    uint8_t actual; 
    uint8_t expect; 

    printf("\n%s started\n", argv[0]);

    bit = 0;
    expect = 1;
    actual = BV_BIT_VAL(digit, bit);
    checkResult(bit, expect, actual);
   
    bit = 1;
    expect = 0;
    actual = BV_BIT_VAL(digit, bit);
    checkResult(bit, expect, actual);

    bit = 2;
    expect = 1;
    actual = BV_BIT_VAL(digit, bit);
    checkResult(bit, expect, actual);

    bit = 7;
    BV_SET_BIT(digit, bit);
    expect = 1;
    actual = BV_BIT_VAL(digit, bit);
    checkResult(bit, expect, actual);

    bit = 7;
    BV_CLEAR_BIT(digit, bit);
    expect = 0;
    actual = BV_BIT_VAL(digit, bit);
    checkResult(bit, expect, actual);

    printf("%s passed\n", argv[0]);
}
