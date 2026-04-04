#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

// --------------------------------
// CRYPTOGRAFIA
// --------------------------------

#define LOW 448
#define MEDIUM 512
#define HIGH 1024
#define LENGTH_MESSAGE 64
#define LENGTH_WORDS_SHA256 32

#define LOW_SHA512 896
#define MEDIUM_SHA512 1024
#define HIGH_SHA512 2048
#define LENGTH_MESSAGE_SHA512 128
#define LENGTH_WORDS_SHA512 64

// --------------------------------
// NUMERI RANDOM
// --------------------------------

#define N 624
#define M 397
#define W 32
#define R 31
#define UMASK (0xffffffffUL << R)
#define LMASK (0xffffffffUL >> (W - R))
#define A 0x9908b0dfUL
#define U 11
#define S 7
#define T 15
#define L 18
#define B 0x9d2c5680UL
#define C 0xefc60000UL
#define F 1812433253UL

typedef struct
{
    const char *input_word; // input

    uint8_t *bits; // bit della parola trasformata
    int length_bit;

    size_t length; // lunghezza della parola

    size_t hex_length; // lunghezza esadecimale
    char *hex_value;   // valore esadecimale

    uint8_t **chunks_bits; // blocchi di bit
    int Num_of_chunks;

    uint8_t *process_message_bit; // bit processo del messaggio
} Sha;

struct sha256
{
    Sha *sha_base;
    char digest[65]; // output
};

struct sha512
{
    Sha *sha_base;
    char digest[129]; // output
};

struct Random_num
{
    uint32_t seed;
    int number;

    int state_index;
    uint32_t state[N];
};
