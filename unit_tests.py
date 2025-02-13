#!/usr/bin/python3

import ctypes
import unittest

# Define the FPE_KEY structure
class FPE_KEY(ctypes.Structure):
    _fields_ = [
        ("key_length", ctypes.c_int),
        ("key", ctypes.c_ubyte * 32),   # 32-byte array
        ("tweak", ctypes.c_ubyte * 16), # 16-byte array
        ("radix", ctypes.c_uint)
    ]

class TestFPE(unittest.TestCase):

    def setUp(self):

        # Load the shared library
        self.fpe_lib = ctypes.CDLL("./libfpe.dylib")

        # Define function signatures
        self.fpe_lib.FPE_ff1_create_key.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_uint]
        self.fpe_lib.FPE_ff1_create_key.restype = ctypes.POINTER(FPE_KEY)  # Returns a pointer to FPE_KEY

        self.fpe_lib.FPE_ff1_encrypt.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.POINTER(FPE_KEY)]
        self.fpe_lib.FPE_ff1_encrypt.restype = None  # void function

        self.fpe_lib.FPE_ff1_delete_key.argtypes = [ctypes.POINTER(FPE_KEY)]
        self.fpe_lib.FPE_ff1_delete_key.restype = None


    def test_one(self):
        print('starting')

        radix = 10
        key = b"EF4359D8D580AA4F7F036D6F04FC6A94"
        tweak = b"D8E7920AFA330A73"
        plaintext = b"890121234567890000" 
        ciphertext = b"318181603547192051"

        # Create FPE key
        fpe_key_ptr = self.fpe_lib.FPE_ff1_create_key(key, tweak, radix)

        # Allocate ciphertext buffer (same size as plaintext)
        cipher = ctypes.create_string_buffer(len(plaintext))
        decrypted = ctypes.create_string_buffer(len(plaintext))

        # Encrypt the plaintext
        self.fpe_lib.FPE_ff1_encrypt(plaintext, cipher, fpe_key_ptr)

        # Decrypt the plaintext
        self.fpe_lib.FPE_ff1_decrypt(cipher, decrypted, fpe_key_ptr)

        # Free the FPE key
        self.fpe_lib.FPE_ff1_delete_key(fpe_key_ptr)

        print(f'FF3 case #: 1')
        print(f'plaintext: {plaintext.decode()}')
        print(f'ciphertext: {cipher.value.decode()}')
        print(f'decrypted: {decrypted.value.decode()}')
        self.assertEqual(ciphertext.decode(), cipher.value.decode())
        self.assertEqual(plaintext.decode(), decrypted.value.decode())

if __name__ == '__main__':
    unittest.main()
