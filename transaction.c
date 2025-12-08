#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.c"
#include <stdint.h>
#include <stddef.h>

// https://learnmeabitcoin.com/technical/transaction/

/*
private key: 7ce1e22db4aad7f5acd824274fb7b667ee043335704086d14b4304d7773915cb, master chain: b933a96ad026819afb1c0df85639c767f4b36152ca50e260da9af9b4ed74d975
chiave pubblica: 048798252534f2fbeb6cb3d08a504042babae188736588ec6e8f8011452707d368c18bfcd0a7bdb4c53f73c9fccdbfa9ba8129f9e95eff368213b6e00b99e87ae2
chiave pubblica compressa:028798252534f2fbeb6cb3d08a504042babae188736588ec6e8f8011452707d368
punto x in SECP256k1: 028798252534f2fbeb6cb3d08a504042babae188736588ec6e8f8011452707d368
punto y in SECP256k1: c18bfcd0a7bdb4c53f73c9fccdbfa9ba8129f9e95eff368213b6e00b99e87ae2
ADDRESS: b'mwc6H8nvFqjNJypGSSsQxf6iZEGNHsW6ia'

3dba85d707442dd2bcb89934688e69ff63a91bd275d9780b50c0b4a3d9cdfaa7
spedire soldi a n2jNQhRTHz377juqkNCATovwGAnjmssDNt
*/

#define OP_PUSHBYTES_65 "65"
#define OP_CHECKSIG "172"

#define PUBLICKEY "048798252534f2fbeb6cb3d08a504042babae188736588ec6e8f8011452707d368c18bfcd0a7bdb4c53f73c9fccdbfa9ba8129f9e95eff368213b6e00b99e87ae2"
#define PRIVATEKEY "7ce1e22db4aad7f5acd824274fb7b667ee043335704086d14b4304d7773915cb"
#define VERSION "02000000"
#define MARKER "00"                                                                    // si utilizza solo quando si è richiesto di sbloccar degli input da wallet di tipo P2WPKH, P2WSH, P2TR
#define FLAG "01"                                                                      // flag per transazioni witness
#define INPUT_COUNT "01"                                                               // quantità di input della transaction
#define TRANSACTION "3dba85d707442dd2bcb89934688e69ff63a91bd275d9780b50c0b4a3d9cdfaa7" // transazione da cui prendere i BTC
#define SEQUENCE "fdffffff"                                                            // quando si vuole spendere i BTC
#define VOUT "01000000"                                                                // indice dell'output che vuoi spendere, in una transazione si possono avere più output, e si possono decidere quale spendere

char transaction[200];

struct Inputs
{
    uint8_t Txid[32];          // transazione da dove vuoi prelevare i BTC
    uint32_t Vout;             // quale output della transazione vuoi spendere (perchè ogni transazione può avere più di un output)
    char ScriptSigSize;        // grandezza scipt sig
    uint8_t *Scriptsig;        // composizione dello scriptsig che serve per spendere l'input
    uint32_t Sequence;         // quando spendere gli input (settaggio di un tempo)
    uint8_t *InputTransaction; // composizione degli input
};

struct Outputs
{
    char Amount[16];
    char ScriptPubKeySize[2];
    char ScriptPubKey[50];
};

struct Wallet
{
    char address[34];
    const char *type_address[10];
    int amount;
    char publicKey[130];
    char privateKey[64];
};

void TransactionCreation(struct Inputs *input) {
    //*input->InputTransaction = (const char *)input->Txid;
    // sprintf(*input->InputTransaction, "%c%c%c%c%c", (const char *)input->Txid, (const char *)input->Vout, input->ScriptSigSize, (const char *)input->Scriptsig, *input->Sequence);
    // sprintf((char)*input->InputTransaction, "%c", (unsigned char)*input->Txid);
};

int main()
{
    struct Wallet user_1;
    struct Wallet user_2;
    struct Inputs inputTransaction;

    int amount_to_transfer = 50;
    user_1.amount = 50;
    user_2.amount = 0;

    // // da questo address mwc6H8nvFqjNJypGSSsQxf6iZEGNHsW6ia --> a questo address n2jNQhRTHz377juqkNCATovwGAnjmssDNt
    // strcpy(user_1.address, "mwc6H8nvFqjNJypGSSsQxf6iZEGNHsW6ia");
    // strcpy(user_1.publicKey, PUBLICKEY);
    // strcpy(user_1.privateKey, PRIVATEKEY);
    // strcpy(user_2.address, "n2jNQhRTHz377juqkNCATovwGAnjmssDNt");

    // strcpy(inputTransaction.Txid, TRANSACTION);
    // strcpy(inputTransaction.Vout, VOUT);
    // strcpy(inputTransaction.Sequence, SEQUENCE);
    // sprintf(inputTransaction.Scriptsig, "%s%s%s", OP_PUSHBYTES_65, user_1.publicKey, OP_CHECKSIG);
    // snprintf(inputTransaction.ScriptSigSize, sizeof(inputTransaction.ScriptSigSize), "%ld", strlen(inputTransaction.Scriptsig));

    // TransactionCreation(&inputTransaction);
    // *user_1.type_address = type_adress(user_1.address);
    // *user_2.type_address = type_adress(user_2.address);
    printf("address1 --> %s, type_address -->  %s\n", user_1.address, *user_1.type_address);
    printf("address2 --> %s, type_address -->  %s\n", user_2.address, *user_2.type_address);
    printf("scriptsig --> %s\n", inputTransaction.Scriptsig);
    printf("scriptsig --> %s\n", *inputTransaction.InputTransaction);
}
