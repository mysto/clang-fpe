[![Build Status](https://app.travis-ci.com/mysto/clang-fpe.svg?branch=master)](https://app.travis-ci.com/mysto/clang-fpe)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)


# FPE - Format Preserving Encryption Implementation in C

An implementation of the NIST approved FF1, FF3 and FF3-1 Format Preserving Encryption (FPE) algorithms in Python.

This package implements the FPE algorithm for Format Preserving Encryption as described in the March 2016 NIST publication 800-38G _Methods for Format-Preserving Encryption_,
and revised on February 28th, 2019 with a draft update for FF3-1.

* [NIST Recommendation SP 800-38G (FF3)](http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38G.pdf)
* [NIST Recommendation SP 800-38G Revision 1 (FF3-1)](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38Gr1-draft.pdf)

## Build and Run

To compile the example.c with the fpe library, just run

`make`

To build on MacOS:
```shell
brew install openssl
export CFLAGS="-I$(brew --prefix openssl)/include"
export LDFLAGS="-L$(brew --prefix openssl)/lib"
```
Run the example in
[example.c](https://github.com/0NG/Format-Preserving-Encryption/blob/master/example.c). 

```shell
./example EF4359D8D580AA4F7F036D6F04FC6A94 D8E7920AFA330A73 10 890121234567890000

FF1 ciphertext: 318181603547192051
FF3 ciphertext: 750918814058654607
```
Run the tests

There are official [test vectors](http://csrc.nist.gov/groups/ST/toolkit/examples.html) for both FF1 and FF3 provided by NIST. You can run [test.py](https://github.com/0NG/Format-Preserving-Encryption/blob/master/test.py) with python 3.x.
with a known test vector:

```shell
make test
```
## Example Usage

This implementation is based on OpenSSL's BIGNUM and AES, so you need to install OpenSSL first.

There are several functions for FF1 and FF3 algorithm, respectively.

1. Set and unset FF1 key and tweak

```c++
int FPE_set_ff1_key(const unsigned char *userKey, const int bits, const unsigned char *tweak, const unsigned int tweaklen, const int radix, FPE_KEY *key);

void FPE_unset_ff1_key(FPE_KEY *key);
```

| name     | description                              |
| -------- | ---------------------------------------- |
| userKey  | encryption key (128 bit, 192 bits or 256 bits), represented as a c string |
| bits     | length of userKey (128, 192 or 256)      |
| tweak    | tweak, represented as a c string         |
| tweaklen | the byte length of the tweak             |
| radix    | number of characters in the given alphabet, it must be in [2, 2^16] |
| key      | FPE_KEY structure                        |

2. encrypt or decrypt text using FF1 algorithm

```c++
void FPE_ff1_encrypt(unsigned int *in, unsigned int *out, unsigned int inlen, FPE_KEY *key)
```

| name  | description                              |
| ----- | ---------------------------------------- |
| in    | numeral string to be encrypted, represented as an array of integers |
| out   | encrypted numeral string, represented as an array of integers |
| inlen | the length of input numeral string (in)  |
| key   | FPE_KEY structure that have been set with key and tweak |

3. Set and unset FF3 key and tweak

```c++
int FPE_set_ff3_key(const unsigned char *userKey, const int bits, const unsigned char *tweak, const unsigned int radix, FPE_KEY *key);

void FPE_unset_ff3_key(FPE_KEY *key);
```

| name    | description                              |
| ------- | ---------------------------------------- |
| userKey | encryption key (128 bit, 192 bits or 256 bits), represented as a c string |
| bits    | length of userKey (128, 192 or 256)      |
| tweak   | tweak, represented as a c string (it must be 64 bytes) |
| radix   | number of characters in the given alphabet, it must be in [2, 2^16] |
| key     | FPE_KEY structure                        |

4. encrypt or decrypt text using FF3 algorithm

```c++
void FPE_ff3_encrypt(unsigned int *in, unsigned int *out, unsigned int inlen, FPE_KEY *key);
```

| name  | description                              |
| ----- | ---------------------------------------- |
| in    | numeral string to be encrypted, represented as an array of integers |
| out   | encrypted numeral string, represented as an array of integers |
| inlen | the length of input numeral string (in)  |
| key   | FPE_KEY structure that have been set with key and tweak |

## TODO

1. Performance testing
2. FF3-1
3. Custom alphabet support
