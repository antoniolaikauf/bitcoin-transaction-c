#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "Global.h"

struct bit_len char_to_bit(const char *string)
{
    struct bit_len len_word_bit;
    len_word_bit.length = strlen(string);
    len_word_bit.bit = (uint8_t *)calloc(len_word_bit.length, sizeof(uint8_t));

    for (int char_id = 0; char_id < len_word_bit.length; char_id++)
    {
        char ch = string[char_id];
        for (int id_bit = 7; id_bit >= 0; id_bit--)
        {
            size_t index = (char_id * 8) + (7 - id_bit);            // indice
            len_word_bit.bit[index] = (ch & (1 << id_bit)) ? 1 : 0; // inserimenti bit
            // printf("%d\n", len_word_bit.bit[(char_id * 8) + (7 - id_bit)]);
        }
        // printf("\n");
    }
    return len_word_bit;
}
