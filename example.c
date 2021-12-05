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

    char ciphertext[100];
    char resulttext[100];

    char* key = argv[1];
    char* tweak = argv[2];
    char* plaintext = argv[4];
    int radix = atoi(argv[3]);

    int txtlen = strlen(plaintext),
        tlen = strlen(tweak) / 2;

    FPE_KEY *ff1 = FPE_ff1_create_key(key, tweak, radix);
	FPE_KEY *ff3 = (tlen == 7) ? 
                      FPE_ff3_1_create_key(key, tweak, radix) : 
                      FPE_ff3_create_key(key, tweak, radix);

    //for (int i = 0; i < xlen; ++i)
    //    assert(x[i] < radix);

    FPE_ff1_encrypt(plaintext, ciphertext, ff1);
    printf("FF1 ciphertext: %s\n\n", ciphertext);

    memset(resulttext, 0, txtlen);
    FPE_ff1_decrypt(ciphertext, resulttext, ff1);

    FPE_ff3_encrypt(plaintext, ciphertext, ff3);
    printf("FF3 ciphertext: %s\n\n", ciphertext);

    memset(resulttext, 0, txtlen);
    FPE_ff3_decrypt(ciphertext, plaintext, ff3);

    FPE_ff1_delete_key(ff1);
    FPE_ff3_delete_key(ff3);

    return 0;
}
