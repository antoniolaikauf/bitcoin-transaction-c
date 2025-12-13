#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

const char *type_adress(char address[])
{
    if ((address[0] == 'm') && strlen(address) == 34)
        return "PRPKH";
    else if ((address[0] == 2) && strlen(address) == 35)
        return "P2SH";

    return "tipo address non trovato";
}

int get_script_sig(int value)
{
    int script_sig = value;
    return script_sig;
}

void hex_to_bit(const char *Hex, uint8_t *bytes)
{
    int lenHex = strlen(Hex) / 2;
    for (int hexId = 0; hexId < lenHex; hexId++)
    {
        sscanf(Hex + 2 * hexId, "%2hhx", &bytes[(lenHex - 1) - hexId]); // togliere -1 perchè sforerebbe fuori
    }
}

void check_value(const uint8_t *bytes, int len) // Aggiungi len come parametro!
{
    for (int i = 0; i < len; i++)
    {
        printf("%02hhx", bytes[i]);
    }
    printf("\n");
}