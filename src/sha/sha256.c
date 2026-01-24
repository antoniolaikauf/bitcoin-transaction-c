#include "../helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* The values are the first 32 bits of the fractional parts of the square roots of the first 8 primes.
 es. sqrt(2) --> si prende la parte frazionaria (i numeri dopo la virgola) --> la si moltiplica per 2**32 --> trasformazione in esadecimale
*/

uint32_t Hash[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

/*
first 32 bits of the fractional parts of the cubic roots of the first 64 prime numbers.
radice cubica(2) --> si prende la parte frazionaria (i numeri dopo la virgola) --> la si moltiplica per 2**32 --> trasformazione in esadecimale
*/
uint32_t Key[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

int main()
{
    const char *test = "XO";
    struct Word result = char_to_bit(test);

    for (size_t i = 0; i < result.length_bit; i++)
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

    // lunghezza in bit
    uint8_t message_len_bit[LENGTH_MESSAGE];
    for (int id_bit = 0; id_bit < LENGTH_MESSAGE; id_bit++)
    {
        unsigned long long val = ((unsigned long long)1) << id_bit;
        message_len_bit[id_bit] = (unsigned long long)result.length_bit & val ? 1 : 0;
        printf("message_len_bit[id_bit] --> %d\n", message_len_bit[id_bit]);
    }

    // little_endian(message_len_bit, 64);
    // add at the end a 1 bit
    /*
        1 si aggiungono tot bot 0 in base a quanto sono lunghi i bit della parola che si
          sta processando
        2 si aggiunge 1 nella posizione di result.length_bit
        3 si fa il processo di little_eldian
        4 si uniscono i bit con il padding con il messaggio della lunghezza
        5 si dividono in chunks
    */

    uint8_t *bits_padding;
    if (result.length_bit < LOW)
    {
        printf("minore di 448\n");
        bits_padding = padding(result.bit, LOW, result.length_bit);
        bits_padding[result.length_bit] = 1;
        little_endian(bits_padding, LOW);

        result.process_message_bit = (uint8_t *)calloc(MEDIUM, sizeof(uint8_t));
        memcpy(result.process_message_bit, bits_padding, LOW);
        memcpy(result.process_message_bit + LOW, message_len_bit, LENGTH_MESSAGE);
        chunks(&result, MEDIUM);
    }
    else if ((LOW <= result.length_bit) && (result.length_bit <= MEDIUM))
    {
        printf("minore di 512\n");
        bits_padding = padding(result.bit, HIGH, result.length_bit);
        bits_padding[result.length_bit] = 1;
        little_endian(bits_padding, HIGH);

        result.process_message_bit = (uint8_t *)calloc(HIGH, sizeof(uint8_t));
        memcpy(result.process_message_bit, bits_padding, (HIGH - LENGTH_MESSAGE));
        memcpy(result.process_message_bit + (HIGH - LENGTH_MESSAGE), message_len_bit, LENGTH_MESSAGE);

        chunks(&result, MEDIUM);
    }
    else
    {
        printf("minore maggiore\n");
        int length = result.length_bit;
        while ((length + LENGTH_MESSAGE) % MEDIUM != 0)
            length++;
        bits_padding = padding(result.bit, length, result.length_bit);
        bits_padding[result.length_bit] = 1;

        result.process_message_bit = (uint8_t *)calloc(LENGTH_MESSAGE + length, sizeof(uint8_t));
        memcpy(result.process_message_bit, bits_padding, length);
        memcpy(result.process_message_bit + length, message_len_bit, LENGTH_MESSAGE);

        chunks(&result, MEDIUM);
    }

    for (int i = 0; i < 512; i++)
    {
        // printf("result.process_message_bit --> %d\n", result.process_message_bit[i]);
    }

    for (int i = 0; i < 1; i++)
    {
        // printf("chunk --> %ld \n", i);
        for (int x = 0; x < 512; x++)
        {
            // printf("bit --> %d \n", result.chunks_bits[i][x]);
        }
    }
    return 0;
}