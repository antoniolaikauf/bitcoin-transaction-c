#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define LOW 448
#define MEDIUM 512
#define HIGH 1024
#define LENGTH_MESSAGE 64


struct Word
{
    uint8_t *bit; // bit della parola trasformata
    int length_bit;

    size_t length; // lunghezza della parola

    size_t hex_length; // lunghezza esadecimale
    char *hex_value;   // valore esadecimale

    uint8_t **chunks_bits; // blocchi di bit

    uint8_t *process_message_bit; // bit processo del messaggio
};
