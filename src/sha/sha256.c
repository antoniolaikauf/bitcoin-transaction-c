#include "../helper.h"

int main()
{
    const char *test = "AB";
    struct bit_len result = char_to_bit(test);

    for (size_t i = 0; i < result.length * 8; i++)
    {
        if (i % 8 == 0)
            printf("\n");
        printf("%u \n", result.bit[i]);
    }
    return 0;
}