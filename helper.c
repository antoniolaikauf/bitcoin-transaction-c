#include <string.h>
#include <stdio.h>

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

int little_endian(int *value)
{
    int value_change = 0;
    for (int id_hex = 0; id_hex < 8; id_hex++)
    {
        printf("valore --> %d", value[id_hex]);
    }
    return value_change;
}