from Crypto.Util.number import *
import gmpy2

p = getPrime(1024)
q = gmpy2.next_prime(p)
e = 65537
N = p * q
phiN = (p - 1) * (q - 1)
d = gmpy2.powmod(e, -1, phiN)

flag = open("flag").read()
messages = ["Do U know RSA?", "The format of flag is: SCTF{}", flag]

def encrypt(m):
	msg = bytes_to_long(m)
	ct = gmpy2.powmod(msg, e, N)
	ct = long_to_bytes(ct)
	return ct.encode("hex")

open("ciphertext.txt", "w").write(", ".join(map(encrypt, messages)))
