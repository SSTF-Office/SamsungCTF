from Crypto.Util.number import *
import gmpy2

e = 65537
ct = [bytes_to_long(c.decode("hex")) for c in open("ciphertext.txt").read().split(", ")]
pt = map(bytes_to_long, ["Do U know RSA?", "The format of flag is: SCTF{}"])

k1N = pow(pt[0], e) - ct[0]
k2N = pow(pt[1], e) - ct[1]

N = gmpy2.gcd(k1N, k2N)

assert(gmpy2.powmod(pt[0], e, N) == ct[0])
assert(gmpy2.powmod(pt[1], e, N) == ct[1])
print hex(N)
