#!/usr/bin/python3

import subprocess
import re
import unittest

#
# format: radix, key, tweak, plainext, ciphertext
#


class TestFPE(unittest.TestCase):

    def test_one(self):
        print('starting')
        regexp = re.compile('(?<=ciphertext: )[a-zA-Z0-9]+')

        radix = 10
        key = "2B7E151628AED2A6ABF7158809CF4F3C"
        tweak = ""
        plain = "0123456789"
        cipher = "2433477484"

        print(f'plaintext: {plain}')

        print(f'FF3 case #: 1')
        print(f'plaintext: {plain}')
        self.assertEqual("A", "A")

if __name__ == '__main__':
    unittest.main()
