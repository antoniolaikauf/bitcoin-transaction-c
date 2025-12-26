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
        printf("valore --> %c \n", result.hex_value[i]);
    }

    return 0;
}