#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define LOW 448
#define MEDIUM 512
#define HIGH 1024
#define LENGTH_MESSAGE 64
#define LENGTH_WORDS_SHA256 32

#define LOW_SHA512 896
#define MEDIUM_SHA512 1024
#define HIGH_sha512 2048
#define LENGTH_MESSAGE_SHA512 128
#define LENGTH_WORDS_SHA512 64

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
    uint32_t digest[65]; // output
};

struct sha512
{
    Sha *sha_base;
    uint64_t digest[80];
};
