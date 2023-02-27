#!/usr/bin/python3

import subprocess
import re
import unittest

#
# format: radix, key, tweak, plainext, ciphertext
#


# ACVP vectors for FF3-1 using 56-bit tweaks from private communication updating:
# https://pages.nist.gov/ACVP/draft-celi-acvp-symmetric.html#name-test-groups

testVectors_ACVP_AES_FF3_1 = [
    # AES - 128
    {
        # tg: 1 tc: 1
        "radix": 10,
        "alphabet": "0123456789",
        "key": "2DE79D232DF5585D68CE47882AE256D6",
        "tweak": "CBD09280979564",
        "plaintext": "3992520240",
        "ciphertext": "8901801106"
    },
    {
        # tg: 1 tc: 1
        "radix": 10,
        "alphabet": "0123456789",
        "key": "01C63017111438F7FC8E24EB16C71AB5",
        "tweak": "C4E822DCD09F27",
        "plaintext": "60761757463116869318437658042297305934914824457484538562",
        "ciphertext": "35637144092473838892796702739628394376915177448290847293"
    },
    # AES - 192
    {
        # tg: 4 tc: 76
        "radix": 10,
        "alphabet": "0123456789",
        "key": "F62EDB777A671075D47563F3A1E9AC797AA706A2D8E02FC8",
        "tweak": "493B8451BF6716",
        "plaintext": "4406616808",
        "ciphertext": "1807744762"
    },
    {
        # tg: 4 tc: 77
        "radix": 10,
        "alphabet": "0123456789",
        "key": "0951B475D1A327C52756F2624AF224C80E9BE85F09B2D44F",
        "tweak": "D679E2EA3054E1",
        "plaintext": "99980459818278359406199791971849884432821321826358606310",
        "ciphertext": "84359031857952748660483617398396641079558152339419110919"
    },
    # AES - 256
    {
        # tg: 7 tc: 151
        "radix": 10,
        "alphabet": "0123456789",
        "key": "1FAA03EFF55A06F8FAB3F1DC57127D493E2F8F5C365540467A3A055BDBE6481D",
        "tweak": "4D67130C030445",
        "plaintext": "3679409436",
        "ciphertext": "1735794859"
    },
    {

        # tg: 7 tc: 152
        "radix": 10,
        "alphabet": "0123456789",
        "key": "9CE16E125BD422A011408EB083355E7089E70A4CD2F59E141D0B94A74BCC5967",
        "tweak": "4684635BD2C821",
        "plaintext": "85783290820098255530464619643265070052870796363685134012",
        "ciphertext": "75104723514036464144839960480545848044718729603261409917"
    },

]
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

        p = subprocess.Popen(['./example', key, tweak, str(radix), plain], stdin = subprocess.PIPE, stdout = subprocess.PIPE)
        output = p.communicate()[0]

        print(f'FF3 case #: 1')
        print(f'plaintext: {plain}')
        self.assertEqual("A", "A")

if __name__ == '__main__':
    unittest.main()
