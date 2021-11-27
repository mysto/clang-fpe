#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fpe.h>
#include <fpe_locl.h>

void hex2chars(char hex[], unsigned char result[])
{
    int len = strlen(hex);
    char temp[3];
    temp[2] = 0x00;

    int j = 0;
    for (int i = 0; i < len; i += 2) {
        temp[0] = hex[i];
        temp[1] = hex[i + 1];
        result[j] = (char)strtol(temp, NULL, 16);
        ++j;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: %s <key> <tweak> <radix> <plaintext>\n", argv[0]);
        return 0;
    }

    unsigned char k[100],
                  t[100],
                  result[100];
    int xlen = strlen(argv[4]),
        klen = strlen(argv[1]) / 2,
        tlen = strlen(argv[2]) / 2,
        radix = atoi(argv[3]);
    unsigned int x[100],
                 y[xlen];

    hex2chars(argv[1], k);
    hex2chars(argv[2], t);
    map_chars(argv[4], x);

    for (int i = 0; i < xlen; ++i)
        assert(x[i] < radix);

    FPE_KEY ff1, ff3;

    printf("key:");
    for (int i = 0; i < klen; ++i)    printf(" %02x", k[i]);
    puts("");
    if (tlen)    printf("tweak:");
    for (int i = 0; i < tlen; ++i)    printf(" %02x", t[i]);
    if (tlen)    puts("");

    FPE_create_ff1_key(k, klen * 8, t, tlen, radix, &ff1);
	if (tlen == 7) {
        FPE_create_ff3_1_key(k, klen * 8, t, radix, &ff3);
	} else {
        FPE_create_ff3_key(k, klen * 8, t, radix, &ff3);
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

