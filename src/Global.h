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

struct Word_sha256
{
    uint8_t *bits; // bit della parola trasformata
    int length_bit;

    size_t length; // lunghezza della parola

    size_t hex_length; // lunghezza esadecimale
    char *hex_value;   // valore esadecimale

    uint8_t **chunks_bits; // blocchi di bit
    int Num_of_chunks;

    uint8_t *process_message_bit; // bit processo del messaggio

    uint32_t out[65];

    const char *input_word; // input
};

// perchè fatto uint_8 i chunks