from Crypto.Util.number import *
import gmpy2

e = 65537
ct = [bytes_to_long(c.decode("hex")) for c in open("ciphertext.txt").read().split(", ")]
pt = map(bytes_to_long, ["Do U know RSA?", "The format of flag is: SCTF{}"])

'''
pt[0] ^ e mod N = ct[0]
pt[1] ^ e mod N = ct[1]

pt[0] ^ e - ct[0] = k1 * N
pt[1] ^ e - ct[1] = k2 * N
'''

k1N = pow(pt[0], e) - ct[0]
k2N = pow(pt[1], e) - ct[1]

N = gmpy2.gcd(k1N, k2N)

for i in range(2, 100):
	if N % i == 0 \
	and gmpy2.powmod(pt[0], e, N // i) == ct[0] \
	and gmpy2.powmod(pt[1], e, N // i) == ct[1]:
		print "reduced:", i
		N //= i

assert(gmpy2.powmod(pt[0], e, N) == ct[0])
assert(gmpy2.powmod(pt[1], e, N) == ct[1])

p = gmpy2.isqrt(N)

while True:
	q, r = gmpy2.t_divmod(N, p)
	if (r == 0):
		break
	p += 1

phiN = (p - 1) * (q - 1)
d = gmpy2.powmod(e, -1, phiN)

flag = long_to_bytes(gmpy2.powmod(ct[2], d, N))
print flag
