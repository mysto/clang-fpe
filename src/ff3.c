#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/aes.h>
#include <openssl/bn.h>
#include "fpe.h"
#include "fpe_locl.h"

void rev_bytes(unsigned char X[], int len)
{
    int hlen = len >> 1;
    for (int i = 0; i < hlen; ++i) {
        unsigned char tmp = X[i];
        X[i] = X[len - i - 1];
        X[len - i - 1] = tmp;
    }
    return;
}

// convert numeral string in reverse order to number
void str2num_rev(BIGNUM *Y, const unsigned int *X, unsigned int radix, unsigned int len, BN_CTX *ctx)
{
    BN_CTX_start(ctx);
    BIGNUM *r = BN_CTX_get(ctx),
           *x = BN_CTX_get(ctx);

    BN_set_word(Y, 0);
    BN_set_word(r, radix);
    for (int i = len - 1; i >= 0; --i) {
        // Y = Y * radix + X[i]
        BN_set_word(x, X[i]);
        BN_mul(Y, Y, r, ctx);
        BN_add(Y, Y, x);
    }

    BN_CTX_end(ctx);
    return;
}

// convert number to numeral string in reverse order
void num2str_rev(const BIGNUM *X, unsigned int *Y, unsigned int radix, int len, BN_CTX *ctx)
{
    BN_CTX_start(ctx);
    BIGNUM *dv = BN_CTX_get(ctx),
           *rem = BN_CTX_get(ctx),
           *r = BN_CTX_get(ctx),
           *XX = BN_CTX_get(ctx);

    BN_copy(XX, X);
    BN_set_word(r, radix);
    memset(Y, 0, len << 2);
    
    for (int i = 0; i < len; ++i) {
        // XX / r = dv ... rem
        BN_div(dv, rem, XX, r, ctx);
        // Y[i] = XX % r
        Y[i] = BN_get_word(rem);
        // XX = XX / r
        BN_copy(XX, dv);
    }

    BN_CTX_end(ctx);
    return;
}

void FF3_encrypt(unsigned int *plaintext, unsigned int *ciphertext, FPE_KEY *key, const unsigned char *tweak, unsigned int txtlen)
{
    BIGNUM *bnum = BN_new(),
           *y = BN_new(),
           *c = BN_new(),
           *anum = BN_new(),
           *qpow_u = BN_new(),
           *qpow_v = BN_new();
    BN_CTX *ctx = BN_CTX_new();


    // Calculate split point
    int u = ceil2(txtlen, 1);
    int v = txtlen - u;

    // Split the message
    memcpy(ciphertext, plaintext, txtlen << 2); 
    unsigned int *A = ciphertext;
    unsigned int *B = ciphertext + u;

    pow_uv(qpow_u, qpow_v, key->radix, u, v, ctx);
    unsigned int temp = (unsigned int)ceil(u * log2(key->radix));
    const int b = ceil2(temp, 3);

    unsigned char S[16], P[16];
    unsigned char *Bytes = (unsigned char *)OPENSSL_malloc(b);

    for (int i = 0; i < FF3_ROUNDS; ++i) {
        // i
        unsigned int m;
        if (i & 1) {
            m = v;
            memcpy(P, tweak, 4);
        } else {
            m = u;
            memcpy(P, tweak + 4, 4);
        }
        P[3] ^= i & 0xff;

        str2num_rev(bnum, B, key->radix, txtlen - m, ctx);
        memset(Bytes, 0x00, b);
        int BytesLen = BN_bn2bin(bnum, Bytes);
        BytesLen = BytesLen > 12? 12: BytesLen;
        memset(P + 4, 0x00, 12);
        memcpy(P + 16 - BytesLen, Bytes, BytesLen);

        // iii
        rev_bytes(P, 16);
        AES_encrypt(P, S, &key->aes_enc_ctx);
        rev_bytes(S, 16);

        // iv
        BN_bin2bn(S, 16, y);

        // v
        str2num_rev(anum, A, key->radix, m, ctx);
        if (i & 1)    BN_mod_add(c, anum, y, qpow_v, ctx);
        else    BN_mod_add(c, anum, y, qpow_u, ctx);

        assert(A != B);
        A = (unsigned int *)( (uintptr_t)A ^ (uintptr_t)B );
        B = (unsigned int *)( (uintptr_t)B ^ (uintptr_t)A );
        A = (unsigned int *)( (uintptr_t)A ^ (uintptr_t)B );

        num2str_rev(c, B, key->radix, m, ctx);

    }

    // free the space
    BN_clear_free(anum);
    BN_clear_free(bnum);
    BN_clear_free(c);
    BN_clear_free(y);
    BN_clear_free(qpow_u);
    BN_clear_free(qpow_v);
    BN_CTX_free(ctx);
    OPENSSL_free(Bytes);
    return;
}

void FF3_decrypt(unsigned int *ciphertext, unsigned int *plaintext, FPE_KEY *key, const unsigned char *tweak, unsigned int txtlen)
{
    BIGNUM *bnum = BN_new(),
           *y = BN_new(),
           *c = BN_new(),
           *anum = BN_new(),
           *qpow_u = BN_new(),
           *qpow_v = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    memcpy(plaintext, ciphertext, txtlen << 2);

    // Calculate split point
    int u = ceil2(txtlen, 1);
    int v = txtlen - u;

    // Split the message
    unsigned int *A = ciphertext;
    unsigned int *B = ciphertext + u;

    pow_uv(qpow_u, qpow_v, key->radix, u, v, ctx);
    unsigned int temp = (unsigned int)ceil(u * log2(key->radix));
    const int b = ceil2(temp, 3);

    unsigned char S[16], P[16];
    unsigned char *Bytes = (unsigned char *)OPENSSL_malloc(b);
    for (int i = FF3_ROUNDS - 1; i >= 0; --i) {
        // i
        int m;
        if (i & 1) {
            m = v;
            memcpy(P, tweak, 4);
        } else {
            m = u;
            memcpy(P, tweak + 4, 4);
        }
        P[3] ^= i & 0xff;

        // ii

        str2num_rev(anum, A, key->radix, txtlen - m, ctx);
        memset(Bytes, 0x00, b);
        int BytesLen = BN_bn2bin(anum, Bytes);
        BytesLen = BytesLen > 12? 12: BytesLen;
        memset(P + 4, 0x00, 12);
        memcpy(P + 16 - BytesLen, Bytes, BytesLen);
       
        // iii
        rev_bytes(P, 16);
        memset(S, 0x00, sizeof(S));
        AES_encrypt(P, S, &key->aes_enc_ctx);
        rev_bytes(S, 16);

        // iv
        BN_bin2bn(S, 16, y);

        // v
        str2num_rev(bnum, B, key->radix, m, ctx);
        if (i & 1)    BN_mod_sub(c, bnum, y, qpow_v, ctx);
        else    BN_mod_sub(c, bnum, y, qpow_u, ctx);

        assert(A != B);
        A = (unsigned int *)( (uintptr_t)A ^ (uintptr_t)B );
        B = (unsigned int *)( (uintptr_t)B ^ (uintptr_t)A );
        A = (unsigned int *)( (uintptr_t)A ^ (uintptr_t)B );

        num2str_rev(c, A, key->radix, m, ctx);

    }

    // free the space
    BN_clear_free(anum);
    BN_clear_free(bnum);
    BN_clear_free(c);
    BN_clear_free(y);
    BN_clear_free(qpow_u);
    BN_clear_free(qpow_v);
    BN_CTX_free(ctx);
    OPENSSL_free(Bytes);
    return;
}

int create_ff3_key(const unsigned char *userKey, const int bits, const unsigned char *tweak, unsigned int radix, FPE_KEY *key)
{
    int ret;
    if (bits != 128 && bits != 192 && bits != 256) {
        ret = -1;
        return ret;
    }
    key->tweaklen = 64;
    key->tweak = (unsigned char *)OPENSSL_malloc(8);
    memcpy(key->tweak, tweak, 8);
	key->radix = radix;

    unsigned char tmp[32];
    memcpy(tmp, userKey, bits >> 3);
    rev_bytes(tmp, bits >> 3);
    ret = AES_set_encrypt_key(tmp, bits, &key->aes_enc_ctx);
    return ret;
}

int FPE_create_ff3_key(const char *key, const char *tweak, unsigned int radix, FPE_KEY *keystruct)
{
    unsigned char k[100],
                  t[100];
    int klen = strlen(key) / 2;

    hex2chars(key, k);
    hex2chars(tweak, t);
    return create_ff3_key(k,klen*8,t,radix,keystruct);
}


int create_ff3_1_key(const unsigned char *userKey, const int bits, const unsigned char *tweak, unsigned int radix, FPE_KEY *key)
{
    int ret;
    if (bits != 128 && bits != 192 && bits != 256) {
        ret = -1;
        return ret;
    }
    key->tweaklen = 64;
    key->tweak = (unsigned char *)OPENSSL_malloc(8);
    memcpy(key->tweak, tweak, 7);
	key->radix = radix;

    // FF3-1: transform 56-bit to 64-bit tweak
    unsigned char byte = tweak[3];
    key->tweak[3] = (byte & 0xF0);
    key->tweak[7] = (byte & 0x0F) << 4;

    unsigned char tmp[32];
    memcpy(tmp, userKey, bits >> 3);
    rev_bytes(tmp, bits >> 3);
    ret = AES_set_encrypt_key(tmp, bits, &key->aes_enc_ctx);
    return ret;
}

int FPE_create_ff3_1_key(const char *key, const char *tweak, unsigned int radix, FPE_KEY *keystruct)
{
    unsigned char k[100],
                  t[100];
    int klen = strlen(key) / 2;

    hex2chars(key, k);
    hex2chars(tweak, t);
    return create_ff3_1_key(k,klen*8,t,radix,keystruct);
}

void delete_ff3_key(FPE_KEY *key)
{
    OPENSSL_free(key->tweak);
    OPENSSL_free(key);
}


void FPE_delete_ff3_key(FPE_KEY *key)
{
    OPENSSL_free(key->tweak);
}

void FPE_ff3_encrypt(unsigned int *in, unsigned int *out, unsigned int txtlen, FPE_KEY *key)
{
    FF3_encrypt(in, out, key, key->tweak, txtlen);
}

void FPE_ff3_decrypt(unsigned int *in, unsigned int *out, unsigned int txtlen, FPE_KEY *key)
{
    FF3_decrypt(in, out, key, key->tweak, txtlen);
}
