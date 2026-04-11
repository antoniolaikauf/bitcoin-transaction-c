#include <stdio.h>
#include "../helper.h"

uint32_t Hash[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
uint32_t Hash_bit[5][LENGTH_WORDS_RIMPED_160] = {0};

uint32_t Key[10] = {0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E, 0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000};
uint32_t Key_bit[10][LENGTH_WORDS_RIMPED_160] = {0};

DEFINE_ROTR(uint32_t);
DEFINE_SHIFT(uint32_t);
DEFINE_XOR_ARRAY(uint32_t);
DEFINE_XORXOR_ARRAY(uint32_t);
DEFINE_AND_ARRAY(uint32_t);
DEFINE_NOT_ARRAY(uint32_t);

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

uint32_t *function_chose(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, uint32_t *e, uint32_t *state, int round, int rot)
{

    uint32_t *out = (uint32_t *)calloc(32, sizeof(uint32_t) * LENGTH_MESSAGE_RIMPED_160);
    uint32_t *f = (uint32_t *)calloc(32, sizeof(uint32_t) * LENGTH_MESSAGE_RIMPED_160);

    switch (round)
    {
    case 0:
        f = XORXOR_ARRAY_uint32_t(b, c, d, LENGTH_MESSAGE_RIMPED_160);
        break;

    case 1:
        out = XOR_ARRAY_uint32_t(a, b, LENGTH_WORDS_RIMPED_160);
        break;

    case 2:
        out = XOR_ARRAY_uint32_t(a, b, LENGTH_WORDS_RIMPED_160);
        break;

    case 3:
        out = XOR_ARRAY_uint32_t(a, b, LENGTH_WORDS_RIMPED_160);
        break;

    case 4:
        out = XOR_ARRAY_uint32_t(a, b, LENGTH_WORDS_RIMPED_160);
        break;

    default:
        break;
    }

    uint32_t *temp1 = adder(a, f, LENGTH_WORDS_RIMPED_160);
    uint32_t *temp2 = adder(state, temp1, LENGTH_WORDS_RIMPED_160);
    uint32_t *temp3 = adder(temp2, Hash_bit[0], LENGTH_WORDS_RIMPED_160);
    uint32_t *temp4 = rotr_arr_uint32_t(temp3, rot, LENGTH_WORDS_RIMPED_160);
    uint32_t *temp5 = adder(temp4, e, LENGTH_WORDS_RIMPED_160);
    memcpy(out, temp5, sizeof(uint32_t) * LENGTH_WORDS_RIMPED_160);

    return out;
}

void rimped_160(struct Rimped_160 *input)
{
    char_to_bit(input->sha_base);

    // lunghezza in bit composta da 64
    uint8_t message_len_bit[LENGTH_MESSAGE_RIMPED_160] = {0};
    int val = 0;
    unsigned long long mask = 0;
    while (input->sha_base->length_bit > mask)
    {
        mask = ((unsigned long long)1) << val;
        message_len_bit[val] = (mask & input->sha_base->length_bit) ? 1 : 0;
        // printf("mask --> %d, message_len_bit[id_bit] --> %d, input->sha_base->length_bit --> %d a indice --> %d\n", mask, message_len_bit[val], input->sha_base->length_bit, val);
        val++;
    }

    little_endian(message_len_bit, 64);

    uint8_t *bits_padding;
    if (input->sha_base->length_bit < LOW_RIMPED_160)
    {
        printf("minore di 448\n");
        bits_padding = padding(input->sha_base->bits, LOW, input->sha_base->length_bit);
        bits_padding[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(MEDIUM, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, bits_padding, LOW);
        memcpy(input->sha_base->process_message_bit + LOW, message_len_bit, LENGTH_MESSAGE);
        chunks(input->sha_base, MEDIUM);
    }
    else if ((LOW <= input->sha_base->length_bit) && (input->sha_base->length_bit <= MEDIUM))
    {
        printf("compreso\n");
        bits_padding = padding(input->sha_base->bits, HIGH, input->sha_base->length_bit);
        bits_padding[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(HIGH, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, bits_padding, (HIGH - LENGTH_MESSAGE));
        memcpy(input->sha_base->process_message_bit + (HIGH - LENGTH_MESSAGE), message_len_bit, LENGTH_MESSAGE);

        chunks(input->sha_base, MEDIUM);
    }
    else
    {
        printf("maggiore di 512\n");
        int length = input->sha_base->length_bit;
        // si trova il valore che sia multiplo di MEDIUM - LENGTH_MESSAGE cosi che si possa aggiungere alla fine
        // la lunghezza del messaggio in bit
        while ((length + LENGTH_MESSAGE) % MEDIUM != 0)
            length++;
        bits_padding = padding(input->sha_base->bits, length, input->sha_base->length_bit);
        bits_padding[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(LENGTH_MESSAGE + length, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, bits_padding, length);
        memcpy(input->sha_base->process_message_bit + length, message_len_bit, LENGTH_MESSAGE);

        chunks(input->sha_base, MEDIUM);
    }

    for (int id_K = 0; id_K < 10; id_K++)
    {
        hex_to_bit(Key[id_K], Key_bit[id_K]);
    }

    for (int id_h = 0; id_h < 5; id_h++)
    {
        hex_to_bit(Hash[id_h], Hash_bit[id_h]);
    }

    for (int id_chunk = 0; id_chunk < input->sha_base->Num_of_chunks; id_chunk++)
    {
        uint32_t Words[16][LENGTH_WORDS_RIMPED_160];
        memset(Words, 0, sizeof(Words));

        for (int id_word = 0; id_word < 16; id_word++)
        {
            for (int id_words_bit = 0; id_words_bit < LENGTH_WORDS_RIMPED_160; id_words_bit++)
            {
                Words[id_word][id_words_bit] = input->sha_base->chunks_bits[id_chunk][(id_word * LENGTH_WORDS_RIMPED_160) + id_words_bit];
                printf("%d", Words[id_word][id_words_bit]);
            }
            printf(" words --> %d\n", id_word);
        }

        uint32_t a[32], b[32], c[32], d[32], e[32];

        memcpy(a, Hash_bit[0], sizeof(uint32_t) * 32);
        memcpy(b, Hash_bit[1], sizeof(uint32_t) * 32);
        memcpy(c, Hash_bit[2], sizeof(uint32_t) * 32);
        memcpy(d, Hash_bit[3], sizeof(uint32_t) * 32);
        memcpy(e, Hash_bit[4], sizeof(uint32_t) * 32);

        memcpy(Words[0], function_chose(a, b, c, d, e, Words[0], 0, 11), sizeof(uint32_t) * 32);
        memcpy(Words[1], function_chose(e, a, b, c, d, Words[1], 0, 14), sizeof(uint32_t) * 32);
        memcpy(Words[2], function_chose(d, e, a, b, c, Words[2], 0, 15), sizeof(uint32_t) * 32);
        memcpy(Words[3], function_chose(c, d, e, a, b, Words[3], 0, 12), sizeof(uint32_t) * 32);
        memcpy(Words[4], function_chose(b, c, d, e, a, Words[4], 0, 5), sizeof(uint32_t) * 32);
        memcpy(Words[5], function_chose(a, b, c, d, e, Words[5], 0, 8), sizeof(uint32_t) * 32);
        memcpy(Words[6], function_chose(e, a, b, c, d, Words[6], 0, 7), sizeof(uint32_t) * 32);
        memcpy(Words[7], function_chose(d, e, a, b, c, Words[7], 0, 9), sizeof(uint32_t) * 32);
        memcpy(Words[8], function_chose(c, d, e, a, b, Words[8], 0, 11), sizeof(uint32_t) * 32);
        memcpy(Words[9], function_chose(b, c, d, e, a, Words[9], 0, 13), sizeof(uint32_t) * 32);
        memcpy(Words[10], function_chose(a, b, c, d, e, Words[10], 0, 14), sizeof(uint32_t) * 32);
        memcpy(Words[11], function_chose(e, a, b, c, d, Words[11], 0, 15), sizeof(uint32_t) * 32);
        memcpy(Words[12], function_chose(d, e, a, b, c, Words[12], 0, 6), sizeof(uint32_t) * 32);
        memcpy(Words[13], function_chose(c, d, e, a, b, Words[13], 0, 7), sizeof(uint32_t) * 32);
        memcpy(Words[14], function_chose(b, c, d, e, a, Words[14], 0, 9), sizeof(uint32_t) * 32);
        memcpy(Words[15], function_chose(a, b, c, d, e, Words[15], 0, 8), sizeof(uint32_t) * 32);

        printf("\n");
        for (size_t i = 0; i < 32; i++)
        {
            printf("%d", Words[0][i]);
        }
        printf("\n");
    }
}

int main()
{

    struct Rimped_160 result;

    result.sha_base = (Sha *)calloc(1, sizeof(Sha));
    result.sha_base->input_word = "hello world";
    rimped_160(&result);
    printf("hello word\n");
    return 0;
}