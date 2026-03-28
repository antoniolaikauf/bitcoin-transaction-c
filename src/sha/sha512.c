#include "../helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

DEFINE_ROTR(uint64_t);
DEFINE_SHIFT(uint64_t);
DEFINE_XOR_ARRAY(uint64_t);
DEFINE_XORXOR_ARRAY(uint64_t);
DEFINE_AND_ARRAY(uint64_t);
DEFINE_NOT_ARRAY(uint64_t);

uint64_t Hash[8] = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
                    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};
uint64_t Hash_bit[8][LENGTH_WORDS_SHA512];

uint64_t Key[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
uint64_t Key_bit[80][LENGTH_WORDS_SHA512];

uint64_t *adder(uint64_t *list1, uint64_t *list2, int len)
{
    uint64_t *sums = (uint64_t *)calloc(len, sizeof(uint64_t));

    int c = 0;

    for (int id_bit = (len - 1); id_bit > -1; id_bit--)
    {
        sums[id_bit] = (list1[id_bit] + ((list2[id_bit] + c) % 2)) % 2;
        c = maj(list1[id_bit], list2[id_bit], c);
        // printf("numero bit --> %d     sums[id_bit] --> %d      c --> %d\n", id_bit, sums[id_bit], c);
    }

    return sums;
}

void sha512(struct sha512 *input)
{
    char_to_bit(input->sha_base);
    bit_to_hex(input->sha_base);

    // lunghezza in bit composta da 64
    uint8_t message_len_bit[LENGTH_MESSAGE_SHA512] = {0};
    int val = 0;
    unsigned long long mask = 0;
    while (input->sha_base->length_bit > mask)
    {
        mask = ((unsigned long long)1) << val;
        message_len_bit[val] = input->sha_base->length_bit & mask ? 1 : 0;
        // printf("mask --> %d, message_len_bit[id_bit] --> %d, input->sha_base->length_bit --> %d a indice --> %d\n", mask, message_len_bit[val], input->sha_base->length_bit, val);
        val++;
    }

    little_endian(message_len_bit, LENGTH_MESSAGE_SHA512);

    uint8_t *padding_message;

    if (input->sha_base->length_bit < LOW_SHA512)
    {
        printf("minore di 896\n");
        padding_message = padding(input->sha_base->bits, LOW_SHA512, input->sha_base->length_bit);
        padding_message[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(MEDIUM_SHA512, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, padding_message, LOW_SHA512);
        memcpy(input->sha_base->process_message_bit + LOW_SHA512, message_len_bit, LENGTH_MESSAGE_SHA512);
        chunks(input->sha_base, MEDIUM_SHA512);
    }
    else if ((LOW_SHA512 <= input->sha_base->length_bit) && (input->sha_base->length_bit <= MEDIUM_SHA512))
    {
        printf("minore di 1024\n");
        padding_message = padding(input->sha_base->bits, HIGH_SHA512, input->sha_base->length_bit);
        padding_message[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(HIGH_SHA512, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, padding_message, (HIGH_SHA512 - LENGTH_MESSAGE_SHA512));
        memcpy(input->sha_base->process_message_bit + (HIGH_SHA512 - LENGTH_MESSAGE_SHA512), message_len_bit, LENGTH_MESSAGE_SHA512);

        chunks(input->sha_base, MEDIUM_SHA512);
    }
    else
    {
        printf("maggiore di 1024\n");
        int length = input->sha_base->length_bit;
        while ((length + LENGTH_MESSAGE_SHA512) % MEDIUM_SHA512 != 0)
            length++;
        padding_message = padding(input->sha_base->bits, length, input->sha_base->length_bit);
        padding_message[input->sha_base->length_bit] = 1;

        input->sha_base->process_message_bit = (uint8_t *)calloc(LENGTH_MESSAGE_SHA512 + length, sizeof(uint8_t));
        memcpy(input->sha_base->process_message_bit, padding_message, length);
        memcpy(input->sha_base->process_message_bit + length, message_len_bit, LENGTH_MESSAGE_SHA512);

        chunks(input->sha_base, MEDIUM_SHA512);
    }

    for (int id_K = 0; id_K < 64; id_K++)
    {
        hex_to_bit_64(Key[id_K], Key_bit[id_K]);
    }

    for (int id_h = 0; id_h < 8; id_h++)
    {
        hex_to_bit_64(Hash[id_h], Hash_bit[id_h]);
    }

    for (int id_chunk = 0; id_chunk < input->sha_base->Num_of_chunks; id_chunk++)
    {
        printf("id_chunk --> %d\n", id_chunk);
        uint64_t Words[80][LENGTH_WORDS_SHA512];
        memset(Words, 0, sizeof(Words));

        for (int id_words = 0; id_words < 16; id_words++)
        {
            for (int id_words_bit = 0; id_words_bit < LENGTH_WORDS_SHA512; id_words_bit++)
            {
                Words[id_words][id_words_bit] = input->sha_base->chunks_bits[id_chunk][(id_words * LENGTH_WORDS_SHA512) + id_words_bit];
                printf("%ld", Words[id_words][id_words_bit]);
            }
            printf(" words --> %d\n", id_words);
        }

        for (int id_words = 16; id_words < 80; id_words++)
        {
            uint64_t *s0 = XORXOR_ARRAY_uint64_t(rotr_arr_uint64_t(Words[id_words - 15], 1, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(Words[id_words - 15], 8, LENGTH_WORDS_SHA512), shift_arr_right_uint64_t(Words[id_words - 15], 7, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *s1 = XORXOR_ARRAY_uint64_t(rotr_arr_uint64_t(Words[id_words - 2], 19, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(Words[id_words - 2], 61, LENGTH_WORDS_SHA512), shift_arr_right_uint64_t(Words[id_words - 2], 6, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *W = adder(adder(adder(Words[id_words - 16], s0, LENGTH_WORDS_SHA512), Words[id_words - 7], LENGTH_WORDS_SHA512), s1, LENGTH_WORDS_SHA512);
            memcpy(Words[id_words], W, sizeof(uint64_t) * LENGTH_WORDS_SHA512);
        }

        uint64_t a[32], b[32], c[32], d[32], e[32], f[32], g[32], h[32];

        memcpy(a, Hash_bit[0], sizeof(uint64_t) * 32);
        memcpy(b, Hash_bit[1], sizeof(uint64_t) * 32);
        memcpy(c, Hash_bit[2], sizeof(uint64_t) * 32);
        memcpy(d, Hash_bit[3], sizeof(uint64_t) * 32);
        memcpy(e, Hash_bit[4], sizeof(uint64_t) * 32);
        memcpy(f, Hash_bit[5], sizeof(uint64_t) * 32);
        memcpy(g, Hash_bit[6], sizeof(uint64_t) * 32);
        memcpy(h, Hash_bit[7], sizeof(uint64_t) * 32);

        for (size_t j = 0; j < 80; j++)
        {
            uint64_t *S1 = XORXOR_ARRAY_uint64_t(rotr_arr_uint64_t(e, 14, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(e, 18, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(e, 41, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *ch = XOR_ARRAY_uint64_t(AND_ARRAY_uint64_t(e, f, LENGTH_WORDS_SHA512), AND_ARRAY_uint64_t(e, g, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *temp1 = adder(adder(adder(adder(h, S1, LENGTH_WORDS_SHA512), ch, LENGTH_WORDS_SHA512), Key_bit[j], LENGTH_WORDS_SHA512), Words[j], LENGTH_WORDS_SHA512);
            uint64_t *S0 = XORXOR_ARRAY_uint64_t(rotr_arr_uint64_t(e, 28, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(e, 34, LENGTH_WORDS_SHA512), rotr_arr_uint64_t(e, 39, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *m = XORXOR_ARRAY_uint64_t(AND_ARRAY_uint64_t(a, b, LENGTH_WORDS_SHA512), AND_ARRAY_uint64_t(a, c, LENGTH_WORDS_SHA512), AND_ARRAY_uint64_t(b, c, LENGTH_WORDS_SHA512), LENGTH_WORDS_SHA512);
            uint64_t *temp2 = adder(S0, m, LENGTH_WORDS_SHA512);

            memcpy(h, g, sizeof(uint64_t) * 64);
            memcpy(g, f, sizeof(uint64_t) * 64);
            memcpy(f, e, sizeof(uint64_t) * 64);
            memcpy(e, adder(d, temp1, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
            memcpy(d, c, sizeof(uint64_t) * 64);
            memcpy(c, b, sizeof(uint64_t) * 64);
            memcpy(b, a, sizeof(uint64_t) * 64);
            memcpy(a, adder(temp1, temp2, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        }

        memcpy(Hash_bit[0], adder(Hash_bit[0], a, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[1], adder(Hash_bit[1], b, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[2], adder(Hash_bit[2], c, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[3], adder(Hash_bit[3], d, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[4], adder(Hash_bit[4], e, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[5], adder(Hash_bit[5], f, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[6], adder(Hash_bit[6], g, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
        memcpy(Hash_bit[7], adder(Hash_bit[7], h, LENGTH_WORDS_SHA512), sizeof(uint64_t) * 64);
    }

    int hex_int = 0;
    for (size_t id_h = 0; id_h < 8; id_h++)
    {
        int int_hex = 0;
        char hex_value;
        for (size_t id_h_bit = 0; id_h_bit < 64; id_h_bit++)
        {
            if (Hash_bit[id_h][id_h_bit])
            {
                int_hex |= (1 << (3 - (id_h_bit % 4)));
            }
            if ((id_h_bit % 4) == 3)
            {
                hex_value = (int_hex < 10) ? ('0' + int_hex) : ('a' + (int_hex - 10));
                input->digest[hex_int] = hex_value;
                hex_int++;
                int_hex = 0;
            }
        }
    }
}

int main()
{
    struct sha512 result;

    result.sha_base = (Sha *)calloc(1, sizeof(Sha));
    result.sha_base->input_word = "hello world";

    // 309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f
    // 7fb0ab95707ec6603829abde750fdec42cb42afcee2d746794e871765f25330951164e98533a974540597d00f13895f5e57e03825636af6cb6d5bf1acacd2f9d
    sha512(&result);

    for (size_t i = 0; i < 128; i++)
    {
        printf("%c", result.digest[i]);
    }
    printf("\n");

    return 0;
}