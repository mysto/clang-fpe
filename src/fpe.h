#ifndef HEADER_FPE_H
# define HEADER_FPE_H

# include <openssl/aes.h>

# ifdef __cplusplus
extern "C" {
# endif

# define FF1_ROUNDS 10
# define FF3_ROUNDS 8
# define FF3_TWEAK_SIZE 8

struct fpe_key_st {
    unsigned int tweaklen;
    unsigned char *tweak;
    unsigned int radix;
    AES_KEY aes_enc_ctx;
};

typedef struct fpe_key_st FPE_KEY;

/*** FF1 ***/
int FPE_create_ff1_key(const char *key, const char *tweak, unsigned int radix, FPE_KEY *keystruct);

void FPE_delete_ff1_key(FPE_KEY *key);

void FPE_ff1_encrypt(unsigned int *plaintext, unsigned int *ciphertext, unsigned int txtlen, FPE_KEY *key);
void FPE_ff1_decrypt(unsigned int *ciphertext, unsigned int *plaintext, unsigned int txtlen, FPE_KEY *key);

/*** FF3 ***/
int FPE_create_ff3_key(const char *key, const char *tweak, unsigned int radix, FPE_KEY *keystruct);
int FPE_create_ff3_1_key(const char *key, const char *tweak, unsigned int radix, FPE_KEY *keystruct);

void FPE_delete_ff3_key(FPE_KEY *key);

void FPE_ff3_encrypt(unsigned int *plaintext, unsigned int *ciphertext, unsigned int inlen, FPE_KEY *key);
void FPE_ff3_decrypt(unsigned int *ciphertext, unsigned int *plaintext, unsigned int inlen, FPE_KEY *key);

# ifdef __cplusplus
}
# endif

#endif
