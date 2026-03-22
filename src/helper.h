#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Global.h"

uint8_t *padding(uint8_t *bits, int max_length, int amount)
{
    uint8_t *bits_pad = (uint8_t *)calloc(max_length, sizeof(uint8_t));
    memcpy(bits_pad, bits, amount);
    return bits_pad;
}

struct Word_sha256 char_to_bit(const char *string)
{
    struct Word_sha256 word;
    word.length = strlen(string);
    word.length_bit = strlen(string) * 8;
    // printf("result.length_bit --> %d \n", word.length_bit);
    word.bits = (uint8_t *)calloc(word.length_bit, sizeof(uint8_t));

    for (int char_id = 0; char_id < word.length; char_id++)
    {
        char ch = string[char_id];
        for (int id_bit = 7; id_bit >= 0; id_bit--)
        {
            size_t index = (char_id * 8) + (7 - id_bit);    // indice
            word.bits[index] = (ch & (1 << id_bit)) ? 1 : 0; // inserimenti bit
            // printf("%d\n", word.bit[(char_id * 8) + (7 - id_bit)]);
        }
        // printf("\n");
    }
    return word;
}

void bit_to_hex(struct Word_sha256 *bits)
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
            if (bits->bits[bit_id])
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

void chunks(struct Word_sha256 *word, int chunk_length)
{
    int amount_chunks = word->length_bit / chunk_length; // calcolo quanti bit è composta la word
    // int true_bit_words = word->length_bit;               // quantità corretta di bit delle parole

    if (amount_chunks == 0)
        amount_chunks = 1;

    word->Num_of_chunks = amount_chunks;

    word->chunks_bits = (uint8_t **)calloc(amount_chunks, sizeof(uint8_t *)); // creazione di una matrice

    for (int id_chunk = 0; id_chunk < amount_chunks; id_chunk++)
    {
        word->chunks_bits[id_chunk] = (uint8_t *)calloc(chunk_length + 1, sizeof(uint8_t)); // allocazione di ogni array di chunk

        // printf("chunk --> %d \n\n", amount_chunks);
        for (int id_bit = 0; id_bit < chunk_length; id_bit++)
        {
            word->chunks_bits[id_chunk][id_bit] = word->process_message_bit[id_bit + (id_chunk * chunk_length)];

            // printf("bit_id --> %d, bit --> %d \n", id_bit + (id_chunk * chunk_length), word->chunks_bits[id_chunk][id_bit]);
        }
    }
}

void hex_to_bit(uint32_t Hex, uint32_t Memory[])
{
    for (int id_hex = 0; id_hex < LENGTH_WORDS_SHA256; id_hex++)
    {
        Memory[id_hex] = ((Hex >> (31 - id_hex)) & 1);
    }
}

// swap importanza di bit in little endian quindi il bit meno significativo si trova a destra
void little_endian(uint8_t *array_bit, int len)
{
    for (int id_bit = 0; id_bit < (len / 2); id_bit++)
    {
        uint8_t tmp = array_bit[id_bit];
        array_bit[id_bit] = array_bit[len - 1 - id_bit];
        array_bit[len - 1 - id_bit] = tmp;
    }
}

uint32_t *XOR_ARRAY(uint32_t i_array[], uint32_t j_array[])
{
    uint32_t *ret = (uint32_t *)calloc(32, 32 * sizeof(uint32_t));
    for (int id = 0; id < 32; id++)
    {
        ret[id] = (i_array[id] + j_array[id]) % 2;
    }

    return ret;
}

uint32_t *XORXOR_ARRAY(uint32_t i_array[], uint32_t j_array[], uint32_t l_array[], int length)
{
    uint32_t *ret = (uint32_t *)calloc(length, sizeof(uint32_t));
    for (int id_array = 0; id_array < length; id_array++)
    {
        ret[id_array] = (i_array[id_array] + ((j_array[id_array] + l_array[id_array]) % 2)) % 2;
        ;
    }
    return ret;
}

uint32_t *AND_ARRAY(uint32_t i_array[], uint32_t j_array[])
{
    uint32_t *ret = (uint32_t *)calloc(32, 32 * sizeof(uint32_t));
    for (int id = 0; id < 32; id++)
    {
        ret[id] = (i_array[id] == 1) ? j_array[id] : 0;
    }

    return ret;
}

uint32_t *NOT_ARRAY(uint32_t i_array[])
{
    uint32_t *ret = (uint32_t *)calloc(32, 32 * sizeof(uint32_t));
    for (int id = 0; id < 32; id++)
    {
        ret[id] = (i_array[id] == 1) ? 0 : 1;
    }

    return ret;
}

int maj(int i, int j, int k)
{
    /*
        tra i j e k si vede tra i e j quale tra essi il suo
        valore (0 o 1) compare di più.
        in questo caso k è quello che da il giudizio
        se i è uguale a k e i è diverso da j allora il
        valore di i è quello che compare di più
    */
    return ((i == k) && (i != j)) ? i : j;
}

uint32_t *rotr_arr(uint32_t array[], int index, int length)
{

    /*
    metodo per ruotare un array in base a index
    es.
    index 3
    [1,2,3,4,5] --> [4,5,1,2,3]
    questo avviene rompendo l'array in due parti per poi ricomporlo
    */
    uint32_t *ret = (uint32_t *)calloc(length, sizeof(uint32_t));
    uint32_t *first_half = (uint32_t *)calloc((length - index), sizeof(uint32_t));
    uint32_t *second_half = (uint32_t *)calloc((index), sizeof(uint32_t));

    memcpy(first_half, array, (length - index) * sizeof(uint32_t));
    memcpy(second_half, array + (length - index), index * sizeof(uint32_t));

    memcpy(ret, second_half, index * sizeof(uint32_t));
    memcpy(ret + index, first_half, (length - index) * sizeof(uint32_t));

    free(first_half);
    free(second_half);

    return ret;
}

uint32_t *shift_arr_right(uint32_t array[], int index)
{
    /*
    metodo che viene usato per shiftare un array a destra in base a index
    es.
    index 3
    [1,2,3,4,5] --> [0,0,0,1,2]
    */
    uint32_t *ret = (uint32_t *)calloc(32, sizeof(uint32_t) * 32);
    memcpy(ret + index, array, (32 - index) * sizeof(uint32_t));

    return ret;
}