#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "Global.h"

struct Word char_to_bit(const char *string)
{
    struct Word len_word_bit;
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

void bit_to_hex(struct Word *bits)
{
    bits->hex_length = bits->length * 2;

    bits->hex_value = (char *)calloc(bits->hex_length + 1, sizeof(char));

    size_t bit_id = 0;
    for (size_t id_hex = 0; id_hex < bits->hex_length; id_hex++)
    {

        int int_hex = 0;
        char hex_value;
        for (int id_bit = 0; 3 >= id_bit; id_bit++)
        {
            // controllo se il bit è acceso
            if (bits->bit[bit_id])
            {
                // accumulo spostando il bit
                int_hex |= (1 << id_bit);
            }
            bit_id++;
        }
        hex_value = (int_hex < 10) ? ('0' + int_hex) : ('a' + int_hex - 10);
        bits->hex_value[id_hex] = hex_value;
        // printf("value --> %c, value in int --> %d\n", hex_value, int_hex);
    }
}