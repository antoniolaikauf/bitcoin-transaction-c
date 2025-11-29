#include <stdio.h>
#include <string.h>

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

struct Wallet
{
    char address[34];
    int amount;
};

int main()
{
    struct Wallet user_1;
    struct Wallet user_2;

    user_1.amount = 50;
    user_2.amount = 0;

    // da questo address mwc6H8nvFqjNJypGSSsQxf6iZEGNHsW6ia --> a questo address n2jNQhRTHz377juqkNCATovwGAnjmssDNt
    strcpy(user_1.address, "mwc6H8nvFqjNJypGSSsQxf6iZEGNHsW6ia");
    strcpy(user_2.address, "n2jNQhRTHz377juqkNCATovwGAnjmssDNt");

    printf("hello\n");
}
