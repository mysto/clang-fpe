[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://github.com/mysto/clang-fpe/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mysto/clang-fpe/actions)

<p align="center">
  <a href="https://privacylogistics.com/">
    <img
      alt="Mysto"
      src="https://privacylogistics.com/Mysto-logo.jpg"
    />
  </a>
</p>

NOTE: NIST's February 2025 Draft 2 has entirely withdrawn FF3 from the NIST standard due to published vulnerabilities.

This software is provided for educational and experimental use and comes with no warranty of any kind.
It is intended for developers and researchers familiar with cryptographic standards.

# FPE - Format Preserving Encryption Implementation in C

An implementation of the FF1, FF3, and FF3-1 format-preserving encryption algorithms in C, derived from an upstream project supporting both FF1 and FF3.

This package implements the FF3 and FF3-1 algorithms as specified in NIST Special Publication 800-38G Methods for Format-Preserving Encryption (now withdrawn), and includes the revisions on February 28th, 2019 with a draft update for FF3-1 (now withdrawn).

* [NIST Recommendation SP 800-38G (FF3)](http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38G.pdf)
* [NIST Recommendation SP 800-38G Revision 1 (FF3-1)](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-38Gr1-draft.pdf)
* [NIST SP 800-38G Revision 1(2nd Public Draft)](https://csrc.nist.gov/pubs/sp/800/38/g/r1/2pd)

## FF1 Support (Context)

This C implementation was originally derived from an upstream project that supported both FF1 and FF3. As a result, FF1 functionality remains present in this codebase for historical, experimental, and interoperability purposes.

FF1 support is not actively promoted, and FF1 implementations are outside the scope of the higher-level language bindings maintained in this project. Users should carefully evaluate patent, security, and compliance considerations before using FF1 in any context.

## Build and Run

To compile the example.c with the fpe library, just run

`make`

To build on macOS:
```shell
brew install openssl
export CFLAGS="-I$(brew --prefix openssl)/include"
export LDFLAGS="-L$(brew --prefix openssl)/lib"
```
Run the example in
[example.c](https://github.com/mysto/clang-fpe/blob/master/example.c). 

```shell
./example EF4359D8D580AA4F7F036D6F04FC6A94 D8E7920AFA330A73 10 890121234567890000

plaintext: 890121234567890000

FF1 ciphertext: 318181603547192051
FF1 decrypted:  890121234567890000

FF3 ciphertext: 750918814058654607
FF3 decrypted:  890121234567890000
```

### Testing

Run the tests

There are official [test vectors](http://csrc.nist.gov/groups/ST/toolkit/examples.html) for both FF1 and FF3 provided by NIST. You can run [test.py](https://github.com/mysto/clang-fpe/blob/master/test.py) with python 3.x.
with a known test vector:

```shell
make test
```

### Benchmarks

This library makes a simple set of benchmarks available.

```shell
make benchmark
./benchmark
```

### Build Configuration

By default, this library uses 128-bit integers where supported for implementing the FF3 algorithm, and falls back to OpenSSL's [`bn`](https://docs.openssl.org/1.0.2/man3/bn/) library otherwise. This library makes macros available to customize or override this behavior for nonstandard compilers or certain use cases:

 - `FPE_USE_BIGNUM`: always use `bn` for the FF3 algorithm.
 - `FPE_U128_TYPEDEF=<type>`: define a typedef for an unsigned 128-bit integer to be used by the FF3 algorithm. If set, this will force 128-bit integer mode.

To pass in parameters, set the value of `FPE_CFLAGS`:

```shell
FPE_CFLAGS='-DFPE_USE_BIGNUM' make
FPE_CFLAGS='"-DFPE_U128_TYPEDEF=unsigned __int128"' make
```

## Example Usage

This implementation is based on OpenSSL's BIGNUM and AES, so you need to install OpenSSL first.

There are several functions for FF1 and FF3 algorithm, respectively.

1. Create and delete FF1 key and tweak

```c
FPE_KEY* FPE_ff1_create_key(const char *key, const char *tweak, const unsigned int radix);

void FPE_ff1_delete_key(FPE_KEY *keystruct);
```

| name     | description                              |
| -------- | ---------------------------------------- |
| key  | encryption key (128 bit, 192 bits or 256 bits), represented as a c string |
| tweak    | tweak, represented as a c string         |
| radix    | number of characters in the given alphabet, it must be in [2, 2^16] |
| returns | FPE_KEY structure                        |

2. encrypt or decrypt text using FF1 algorithm

```c
void FPE_ff1_encrypt(char *plaintext, char *ciphertext, FPE_KEY *keystruct)
void FPE_ff1_decrypt(char *ciphertext, char *plaintext, FPE_KEY *keystruct)
```

| name  | description                              |
| ----- | ---------------------------------------- |
| plaintext  | numeral string to be encrypted, represented as an array of integers |
| ciphertext | encrypted numeral string, represented as an array of integers |
| keystruct   | FPE_KEY structure that has been set with key and tweak |

3. Create and delete FF3 key and tweak

```c
FPE_KEY* FPE_ff3_create_key(const char *key, const char *tweak, const unsigned char radix);
FPE_KEY* FPE_ff3_1_create_key(const char *key, const char *tweak, const unsigned char radix);

void FPE_ff3_delete_key(FPE_KEY *keystruct);
```

| name    | description                              |
| ------- | ---------------------------------------- |
| key | encryption key (128 bit, 192 bits or 256 bits), represented as a c string |
| tweak   | tweak, represented as a c string (it must be 64 bits) |
| radix    | number of characters in the given alphabet, it must be in [2, 62] |
| returns | FPE_KEY structure                        |

4. encrypt or decrypt text using FF3 algorithm

```c
void FPE_ff3_encrypt(char *plaintext, char *ciphertext, FPE_KEY *keystruct);
void FPE_ff3_decrypt(char *ciphertext, char *plaintext, FPE_KEY *keystruct);
```

| name  | description                              |
| ----- | ---------------------------------------- |
| plaintext  | numeral string to be encrypted, represented as an array of integers |
| ciphertext | encrypted numeral string, represented as an array of integers |
| keystruct   | FPE_KEY structure that has been set with key and tweak |

## TODO

1. Performance testing
3. Custom alphabet support

## License

This C implementation is licensed under the [MIT license](https://mit-license.org); other language bindings may use different licenses.”

