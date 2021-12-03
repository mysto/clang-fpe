#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fpe.h>
#include <fpe_locl.h>

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: %s <key> <tweak> <radix> <plaintext>\n", argv[0]);
        return 0;
    }

    unsigned char result[100];

    char* key = argv[1];
    char* tweak = argv[2];
    char* plaintext = argv[4];
    int radix = atoi(argv[3]);

    int xlen = strlen(plaintext),
        tlen = strlen(tweak) / 2;

    unsigned int x[100],
                 y[xlen];

    map_chars(argv[4], x);

    for (int i = 0; i < xlen; ++i)
        assert(x[i] < radix);

    FPE_KEY ff1, ff3;

    //printf("key:");
    //for (int i = 0; i < klen; ++i)    printf(" %02x", k[i]);
    //puts("");
    //if (tlen)    printf("tweak:");
    //for (int i = 0; i < tlen; ++i)    printf(" %02x", t[i]);
    //if (tlen)    puts("");

    FPE_create_ff1_key(key, tweak, radix, &ff1);
	if (tlen == 7) {
        FPE_create_ff3_1_key(key, tweak, radix, &ff3);
	} else {
        FPE_create_ff3_key(key, tweak, radix, &ff3);
    }

    printf("after map: ");
    for (int i = 0; i < xlen; ++i)    printf(" %d", x[i]);
    printf("\n\n");

    printf("plaintext: ");
    for (int i = 0; i < xlen; ++i)    printf(" %d", x[i]);
    printf("\n\n");

    FPE_ff1_encrypt(x, y, xlen, &ff1);

    inverse_map_chars(y, result, xlen);
    printf("FF1 ciphertext: %s\n\n", result);

    memset(x, 0, sizeof(x));
    FPE_ff1_decrypt(y, x, xlen, &ff1);

    FPE_ff3_encrypt(x, y, xlen, &ff3);
    inverse_map_chars(y, result, xlen);
    printf("FF3 ciphertext: %s\n\n", result);

    memset(x, 0, sizeof(x));
    FPE_ff3_decrypt(y, x, xlen, &ff3);

    FPE_delete_ff1_key(&ff1);
    FPE_delete_ff3_key(&ff3);

    return 0;
}

