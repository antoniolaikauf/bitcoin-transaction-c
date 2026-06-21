#include <stdio.h>
#include <time.h>
#include <openssl/rand.h> // ← Libreria OpenSSL
#include "../sha/sha256.c"

#define PRIVATE_KEY_SIZE 32

struct PrivateKey
{
    uint8_t randArr[PRIVATE_KEY_SIZE + 1];
};

struct PublicKey
{
};

struct Key
{
    struct PrivateKey PrivateK;
    struct PublicKey PublicK;
    uint8_t prefisso;
};

int Generate_pivate_key(struct Key *chiavi)
{
    chiavi->prefisso = 0x80; // prefisso per il WIF
    chiavi->PrivateK.randArr[0] = chiavi->prefisso;
    // array di 32 bit composto da numeri randomici, ogni numero ha 8 bit
    if (RAND_bytes(chiavi->PrivateK.randArr + 1, PRIVATE_KEY_SIZE) != 1)
        return 0;

    for (size_t index = 0; index < PRIVATE_KEY_SIZE; index++)
    {
        printf("%02x", chiavi->PrivateK.randArr[index]);
    }

    struct sha256 FirstSha;
    FirstSha.sha_base = (Sha *)calloc(1, sizeof(Sha));
    FirstSha.sha_base->input_word = (const char *)chiavi->PrivateK.randArr;
    sha256(&FirstSha);

    struct sha256 SecondSha;
    SecondSha.sha_base = (Sha *)calloc(1, sizeof(Sha));
    SecondSha.sha_base->input_word = FirstSha.digest;
    sha256(&SecondSha);

    // cheksum
    uint8_t *checksum[4];
    memcpy(checksum, (uint8_t *)SecondSha.digest, 4);

    uint8_t final_key[37];

    // Copia private key
    memcpy(final_key, chiavi->PrivateK.randArr, 33);

    // Aggiungi i 4 byte di checksum
    memcpy(final_key + 33, checksum, 4);

    for (int i = 0; i < 37; i++)
        printf("%02x", final_key[i]);
    printf("\n");

    free(FirstSha.sha_base);
    free(SecondSha.sha_base);
    return 1;


    // https://medium.com/@raphaeldemoraesdutenkefer/implementing-a-base58-decoder-for-extended-private-key-in-c-d4cc8e9b0792
    
}

int main()
{

    srand(time(NULL));
    struct Key chiavi;

    if (Generate_pivate_key(&chiavi) != 1)
    {
        printf("errore generazione della chiave");
    }
    return 0;
}

// gcc -o Key Key.c -lcrypto per compilare