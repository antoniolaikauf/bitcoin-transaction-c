#include "../helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

DEFINE_ROTR(uint32_t);
DEFINE_SHIFT(uint32_t);
DEFINE_XOR_ARRAY(uint32_t);
DEFINE_XORXOR_ARRAY(uint32_t);
DEFINE_AND_ARRAY(uint32_t);
DEFINE_NOT_ARRAY(uint32_t);
/* The values are the first 32 bits of the fractional parts of the square roots of the first 8 primes.
 es. sqrt(2) --> si prende la parte frazionaria (i numeri dopo la virgola) --> la si moltiplica per 2**32 --> trasformazione in esadecimale
*/

uint32_t Hash[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
uint32_t Hash_bit[8][LENGTH_WORDS_SHA256];
/*
first 32 bits of the fractional parts of the cubic roots of the first 64 prime numbers.
radice cubica(2) --> si prende la parte frazionaria (i numeri dopo la virgola) --> la si moltiplica per 2**32 --> trasformazione in esadecimale
*/
uint32_t Key[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
uint32_t Key_bit[64][LENGTH_WORDS_SHA256];

uint32_t *adder(uint32_t *list1, uint32_t *list2, int len)
{
    uint32_t *sums = (uint32_t *)calloc(len, sizeof(uint32_t));

    int c = 0;

    for (int id_bit = (len - 1); id_bit > -1; id_bit--)
    {
        sums[id_bit] = (list1[id_bit] + ((list2[id_bit] + c) % 2)) % 2;
        c = maj(list1[id_bit], list2[id_bit], c);
        // printf("numero bit --> %d     sums[id_bit] --> %d      c --> %d\n", id_bit, sums[id_bit], c);
    }

    return sums;
}

void sha256(struct sha256 *result)
{
    char_to_bit(result->sha_base);
    bit_to_hex(result->sha_base);
    // lunghezza in bit composta da 64 bit
    uint8_t message_len_bit[LENGTH_MESSAGE] = {0};
    int val = 0;
    unsigned long long mask = 0;
    while (result->sha_base->length_bit > mask)
    {
        mask = ((unsigned long long)1) << val;
        message_len_bit[val] = result->sha_base->length_bit & mask ? 1 : 0;
        // printf("mask --> %d, message_len_bit[id_bit] --> %d, result->sha_base->length_bit --> %d a indice --> %d\n", mask, message_len_bit[val], result->sha_base->length_bit, val);
        val++;
    }

    little_endian(message_len_bit, 64);
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
    if (result->sha_base->length_bit < LOW)
    {
        printf("minore di 448\n");
        bits_padding = padding(result->sha_base->bits, LOW, result->sha_base->length_bit);
        bits_padding[result->sha_base->length_bit] = 1;

        result->sha_base->process_message_bit = (uint8_t *)calloc(MEDIUM, sizeof(uint8_t));
        memcpy(result->sha_base->process_message_bit, bits_padding, LOW);
        memcpy(result->sha_base->process_message_bit + LOW, message_len_bit, LENGTH_MESSAGE);
        chunks(result->sha_base, MEDIUM);
    }
    else if ((LOW <= result->sha_base->length_bit) && (result->sha_base->length_bit <= MEDIUM))
    {
        printf("minore di 512\n");
        bits_padding = padding(result->sha_base->bits, HIGH, result->sha_base->length_bit);
        bits_padding[result->sha_base->length_bit] = 1;

        result->sha_base->process_message_bit = (uint8_t *)calloc(HIGH, sizeof(uint8_t));
        memcpy(result->sha_base->process_message_bit, bits_padding, (HIGH - LENGTH_MESSAGE));
        memcpy(result->sha_base->process_message_bit + (HIGH - LENGTH_MESSAGE), message_len_bit, LENGTH_MESSAGE);

        chunks(result->sha_base, MEDIUM);
    }
    else
    {
        printf("maggiore di 512\n");
        int length = result->sha_base->length_bit;
        while ((length + LENGTH_MESSAGE) % MEDIUM != 0)
            length++;
        bits_padding = padding(result->sha_base->bits, length, result->sha_base->length_bit);
        bits_padding[result->sha_base->length_bit] = 1;

        result->sha_base->process_message_bit = (uint8_t *)calloc(LENGTH_MESSAGE + length, sizeof(uint8_t));
        memcpy(result->sha_base->process_message_bit, bits_padding, length);
        memcpy(result->sha_base->process_message_bit + length, message_len_bit, LENGTH_MESSAGE);

        chunks(result->sha_base, MEDIUM);
    }

    for (int id_K = 0; id_K < 64; id_K++)
    {
        hex_to_bit(Key[id_K], Key_bit[id_K]);
    }

    for (int id_h = 0; id_h < 8; id_h++)
    {
        hex_to_bit(Hash[id_h], Hash_bit[id_h]);
    }

    for (int id_chunk = 0; id_chunk < result->sha_base->Num_of_chunks; id_chunk++)
    {
        uint32_t Words[64][LENGTH_WORDS_SHA256];
        memset(Words, 0, sizeof(Words));
        //  si copia i primi 16 word
        //  si aggiungono 48 words contenenti 0 bit

        for (int id_words = 0; id_words < 16; id_words++)
        {
            for (int id_words_bit = 0; id_words_bit < LENGTH_WORDS_SHA256; id_words_bit++)
            {
                Words[id_words][id_words_bit] = result->sha_base->chunks_bits[id_chunk][(id_words * LENGTH_WORDS_SHA256) + id_words_bit];
                // printf("%d", Words[id_words][id_words_bit]);
            }
            // printf(" words --> %d\n", id_words);
        }

        for (int id = 16; id < 64; id++)
        {
            uint32_t *s0 = XORXOR_ARRAY_uint32_t(rotr_arr_uint32_t(Words[id - 15], 7, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(Words[id - 15], 18, LENGTH_WORDS_SHA256), shift_arr_right_uint32_t(Words[id - 15], 3, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *s1 = XORXOR_ARRAY_uint32_t(rotr_arr_uint32_t(Words[id - 2], 17, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(Words[id - 2], 19, LENGTH_WORDS_SHA256), shift_arr_right_uint32_t(Words[id - 2], 10, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *W = adder(adder(adder(Words[id - 16], s0, LENGTH_WORDS_SHA256), Words[id - 7], LENGTH_WORDS_SHA256), s1, LENGTH_WORDS_SHA256);
            memcpy(Words[id], W, sizeof(uint32_t) * LENGTH_WORDS_SHA256);
        }

        uint32_t a[32], b[32], c[32], d[32], e[32], f[32], g[32], h[32];

        memcpy(a, Hash_bit[0], sizeof(uint32_t) * 32);
        memcpy(b, Hash_bit[1], sizeof(uint32_t) * 32);
        memcpy(c, Hash_bit[2], sizeof(uint32_t) * 32);
        memcpy(d, Hash_bit[3], sizeof(uint32_t) * 32);
        memcpy(e, Hash_bit[4], sizeof(uint32_t) * 32);
        memcpy(f, Hash_bit[5], sizeof(uint32_t) * 32);
        memcpy(g, Hash_bit[6], sizeof(uint32_t) * 32);
        memcpy(h, Hash_bit[7], sizeof(uint32_t) * 32);

        for (int j = 0; j < 64; j++)
        {
            uint32_t *S1 = XORXOR_ARRAY_uint32_t(rotr_arr_uint32_t(e, 6, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(e, 11, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(e, 25, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *ch = XOR_ARRAY_uint32_t(AND_ARRAY_uint32_t(e, f, LENGTH_WORDS_SHA256), AND_ARRAY_uint32_t(NOT_ARRAY_uint32_t(e, LENGTH_WORDS_SHA256), g, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *temp1 = adder(adder(adder(adder(h, S1, LENGTH_WORDS_SHA256), ch, LENGTH_WORDS_SHA256), Key_bit[j], LENGTH_WORDS_SHA256), Words[j], LENGTH_WORDS_SHA256);
            uint32_t *S0 = XORXOR_ARRAY_uint32_t(rotr_arr_uint32_t(a, 2, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(a, 13, LENGTH_WORDS_SHA256), rotr_arr_uint32_t(a, 22, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *m = XORXOR_ARRAY_uint32_t(AND_ARRAY_uint32_t(a, b, LENGTH_WORDS_SHA256), AND_ARRAY_uint32_t(a, c, LENGTH_WORDS_SHA256), AND_ARRAY_uint32_t(b, c, LENGTH_WORDS_SHA256), LENGTH_WORDS_SHA256);
            uint32_t *temp2 = adder(S0, m, LENGTH_WORDS_SHA256);

            memcpy(h, g, sizeof(uint32_t) * 32);
            memcpy(g, f, sizeof(uint32_t) * 32);
            memcpy(f, e, sizeof(uint32_t) * 32);

            memcpy(e, adder(d, temp1, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);

            memcpy(d, c, sizeof(uint32_t) * 32);
            memcpy(c, b, sizeof(uint32_t) * 32);
            memcpy(b, a, sizeof(uint32_t) * 32);

            memcpy(a, adder(temp1, temp2, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        }

        memcpy(Hash_bit[0], adder(Hash_bit[0], a, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[1], adder(Hash_bit[1], b, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[2], adder(Hash_bit[2], c, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[3], adder(Hash_bit[3], d, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[4], adder(Hash_bit[4], e, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[5], adder(Hash_bit[5], f, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[6], adder(Hash_bit[6], g, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
        memcpy(Hash_bit[7], adder(Hash_bit[7], h, LENGTH_WORDS_SHA256), sizeof(uint32_t) * 32);
    }

    int hex_index = 0;
    for (int id_h = 0; id_h < 8; id_h++)
    {
        int int_hex = 0;
        char hex_value;
        for (int id_h_bit = 0; id_h_bit < 32; id_h_bit++)
        {
            if (Hash_bit[id_h][id_h_bit])
            {
                int_hex |= (1 << (3 - (id_h_bit % 4)));
            }

            if ((id_h_bit % 4) == 3)
            {
                hex_value = (int_hex < 10) ? ('0' + int_hex) : ('a' + (int_hex - 10));
                result->digest[hex_index] = hex_value;
                hex_index++;
                int_hex = 0;
            }
        }
    }
}

// test
int main()
{
    struct sha256 result;
    result.sha_base = (Sha *)calloc(1, sizeof(Sha));
    result.sha_base->input_word = "hello world";

    sha256(&result);
    for (int i = 0; i < 64; i++) // 8 caratteri hex per word
        printf("%c", result.digest[i]);
    printf("\n");
    return 0;
}