#!/usr/bin/python3

import subprocess
import re
import unittest

#
# format: radix, key, tweak, plainext, ciphertext
#

ff1 = [
    # AES-128
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3C",
       "",
       "0123456789",
       "2433477484",
   ],
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3C",
       "39383736353433323130",
       "0123456789",
       "6124200773",
   ],
   [
       36,
       "2B7E151628AED2A6ABF7158809CF4F3C",
       "3737373770717273373737",
       "0123456789abcdefghi",
       "a9tv40mll9kdu509eum",
   ],
   
   # AES-192
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F",
       "",
       "0123456789",
       "2830668132",
   ],
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F",
       "39383736353433323130",
       "0123456789",
       "2496655549",
   ],
   [
       36,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F",
       "3737373770717273373737",
       "0123456789abcdefghi",
       "xbj3kv35jrawxv32ysr",
   ],
   
   # AES-256
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F7F036D6F04FC6A94",
       "",
       "0123456789",
       "6657667009",
   ],
   [
       10,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F7F036D6F04FC6A94",
       "39383736353433323130",
       "0123456789",
       "1001623463",
   ],
   [
       36,
       "2B7E151628AED2A6ABF7158809CF4F3CEF4359D8D580AA4F7F036D6F04FC6A94",
       "3737373770717273373737",
       "0123456789abcdefghi",
       "xs8a0azh2avyalyzuwd",
   ],
]

ff3 = [
    # AES-128
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A94",
        "D8E7920AFA330A73",
        "890121234567890000",
        "750918814058654607",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A94",
        "9A768A92F60E12D8",
        "890121234567890000",
        "018989839189395384",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A94",
        "D8E7920AFA330A73",
        "89012123456789000000789000000",
        "48598367162252569629397416226",
    ],
    [
        10, "EF4359D8D580AA4F7F036D6F04FC6A94",
        "0000000000000000",
        "89012123456789000000789000000",
        "34695224821734535122613701434",
    ],
    [
        26, "EF4359D8D580AA4F7F036D6F04FC6A94",
        "9A768A92F60E12D8",
        "0123456789abcdefghi",
        "g2pk40i992fn20cjakb",
    ],
    
    # AES-192
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6",
        "D8E7920AFA330A73",
        "890121234567890000",
        "646965393875028755",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6",
        "9A768A92F60E12D8",
        "890121234567890000",
        "961610514491424446",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6",
        "D8E7920AFA330A73",
        "89012123456789000000789000000",
        "53048884065350204541786380807",
    ],
    [
        10, "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6",
        "0000000000000000",
        "89012123456789000000789000000",
        "98083802678820389295041483512",
    ],
    [
        26, "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6",
        "9A768A92F60E12D8",
        "0123456789abcdefghi",
        "i0ihe2jfj7a9opf9p88",
    ],
    
    # AES-256
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6ABF7158809CF4F3C",
        "D8E7920AFA330A73",
        "890121234567890000",
        "922011205562777495",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6ABF7158809CF4F3C",
        "9A768A92F60E12D8",
        "890121234567890000",
        "504149865578056140",
    ],
    [
        10,
        "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6ABF7158809CF4F3C",
        "D8E7920AFA330A73",
        "89012123456789000000789000000",
        "04344343235792599165734622699",
    ],
    [
        10, "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6ABF7158809CF4F3C",
        "0000000000000000",
        "89012123456789000000789000000",
        "30859239999374053872365555822",
    ],
    [
        26, "EF4359D8D580AA4F7F036D6F04FC6A942B7E151628AED2A6ABF7158809CF4F3C",
        "9A768A92F60E12D8",
        "0123456789abcdefghi",
        "p0b2godfja9bhb7bk38",
    ],
]

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

class TestFF3(unittest.TestCase):

    def test_vectors_ff1(self):
        regexp = re.compile('(?<=ciphertext: )[a-zA-Z0-9]+')
        for index, test in enumerate(ff1):
            with self.subTest(index=index):
                radix = test[0]
                key = test[1]
                tweak = test[2]
                plain = test[3]
                cipher = test[4]
                p = subprocess.Popen(['./example', key, tweak, str(radix), plain], stdin = subprocess.PIPE, stdout = subprocess.PIPE)
                output = p.communicate()[0]
                results = regexp.findall(output.decode('utf-8'))[0]
                p.wait()

                #print(f'FF1 case #: {index}')
                #print(f'plaintext: {plain}')
                #print(f'ciphertext: {results}')
                self.assertEqual(results, cipher)
    
    def test_vectors_ff3(self):
        regexp = re.compile('(?<=ciphertext: )[a-zA-Z0-9]+')
        for index, test in enumerate(ff3):
            with self.subTest(index=index):
                radix = test[0]
                key = test[1]
                tweak = test[2]
                plain = test[3]
                cipher = test[4]
                p = subprocess.Popen(['./example', key, tweak, str(radix), plain], stdin = subprocess.PIPE, stdout = subprocess.PIPE)
                output = p.communicate()[0]
                results = regexp.findall(output.decode('utf-8'))[1]
                p.wait()

                #print(f'FF3 case #: {index}')
                #print(f'plaintext: {plain}')
                #print(f'ciphertext: {results}')
                self.assertEqual(results, cipher)
    
    def test_encrypt_acvp(self):
        regexp = re.compile('(?<=ciphertext: )[a-zA-Z0-9]+')
        for testVector in testVectors_ACVP_AES_FF3_1:
            with self.subTest(testVector=testVector):
                p = subprocess.Popen(['./example', testVector['key'], testVector['tweak'], str(testVector['radix']), testVector['plaintext']], stdin = subprocess.PIPE, stdout = subprocess.PIPE)
                output = p.communicate()[0]
                results = regexp.findall(output.decode('utf-8'))[1]
                p.wait()
                self.assertEqual(results, testVector['ciphertext'])


if __name__ == '__main__':
    unittest.main()
