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

struct Word char_to_bit(const char *string)
{
    struct Word word;
    word.length = strlen(string);
    word.length_bit = strlen(string) * 8;
    // printf("result.length_bit --> %d \n", word.length_bit);
    word.bit = (uint8_t *)calloc(word.length_bit, sizeof(uint8_t));

    for (int char_id = 0; char_id < word.length; char_id++)
    {
        char ch = string[char_id];
        for (int id_bit = 7; id_bit >= 0; id_bit--)
        {
            size_t index = (char_id * 8) + (7 - id_bit);    // indice
            word.bit[index] = (ch & (1 << id_bit)) ? 1 : 0; // inserimenti bit
            // printf("%d\n", word.bit[(char_id * 8) + (7 - id_bit)]);
        }
        // printf("\n");
    }
    return word;
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

void chunks(struct Word *word, int chunk_length)
{
    int amount_chunks = word->length_bit / chunk_length; // calcolo quanti bit è composta la word
    // int true_bit_words = word->length_bit;               // quantità corretta di bit delle parole

    if (amount_chunks == 0)
        amount_chunks = 1;

    word->chunks_bits = (uint8_t **)calloc(amount_chunks, sizeof(uint8_t *)); // creazione di una matrice

    for (int id_chunk = 0; id_chunk < amount_chunks; id_chunk++)
    {
        word->chunks_bits[id_chunk] = (uint8_t *)calloc(chunk_length + 1, sizeof(uint8_t)); // allocazione di ogni array di chunk

        // printf("chunk --> %d \n\n", amount_chunks);
        for (int id_bit = 0; id_bit < chunk_length; id_bit++)
        {
            word->chunks_bits[id_chunk][id_bit] = word->process_message_bit[id_bit + (id_chunk * chunk_length)]; // allocazione di ogni array

            //  printf("bit_id --> %d, bit --> %d \n", id_bit + (id_chunk * chunk_length), word->chunks_bits[id_chunk][id_bit]);
        }
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

bool if_(int i, int y, int z)
{
    return (i == 1) ? y : z;
}

bool XOR_(int i, int j)
{
    return if_(i, if_(j, 0, 1), j);
}

bool XORXOR_(int i, int j, int l)
{
    return XOR_(i, XOR_(j, l));
}

bool maj(int i, int j, int k)
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
