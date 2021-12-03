#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fpe.h>
#include <fpe_locl.h>

/*
  usage:

  ./example 2DE79D232DF5585D68CE47882AE256D6 CBD09280979564 10 3992520240

*/

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

    FPE_KEY *ff1 = FPE_create_ff1_key(key, tweak, radix);
	FPE_KEY *ff3 = (tlen == 7) ? 
                      FPE_create_ff3_1_key(key, tweak, radix) : 
                      FPE_create_ff3_key(key, tweak, radix);

    map_chars(plaintext, x);

    for (int i = 0; i < xlen; ++i)
        assert(x[i] < radix);

    printf("plaintext: ");
    for (int i = 0; i < xlen; ++i)    printf(" %d", x[i]);
    printf("\n\n");

    FPE_ff1_encrypt(x, y, xlen, ff1);

    inverse_map_chars(y, result, xlen);
    printf("FF1 ciphertext: %s\n\n", result);

    memset(x, 0, sizeof(x));
    FPE_ff1_decrypt(y, x, xlen, ff1);

    FPE_ff3_encrypt(x, y, xlen, ff3);
    inverse_map_chars(y, result, xlen);
    printf("FF3 ciphertext: %s\n\n", result);

    memset(x, 0, sizeof(x));
    FPE_ff3_decrypt(y, x, xlen, ff3);

    FPE_delete_ff1_key(ff1);
    FPE_delete_ff3_key(ff3);

    return 0;
}
