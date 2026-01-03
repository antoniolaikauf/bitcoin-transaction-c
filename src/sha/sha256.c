#include "../helper.h"

int main()
{
    const char *test = "XO";
    struct Word result = char_to_bit(test);

    for (size_t i = 0; i < result.length * 8; i++)
    {
        if (i % 8 == 0)
            printf("\n");
        printf("%u \n", result.bit[i]);
    }

    bit_to_hex(&result);
    for (size_t i = 0; i < result.hex_length; i++)
    {
        // printf("valore --> %c \n", result.hex_value[i]);
    }

    uint8_t **chunks_bits = chunks(result, 8);

    for (size_t i = 0; i < 2; i++)
    {
        // printf("chunk --> %ld \n", i);
        for (size_t x = 0; x < 8; x++)
        {
            // printf("bit --> %d \n", chunks_bits[i][x]);
        }
    }

    return 0;
}