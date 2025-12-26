#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

struct Word
{
    uint8_t *bit;
    size_t length;

    size_t hex_length;
    char *hex_value;
};