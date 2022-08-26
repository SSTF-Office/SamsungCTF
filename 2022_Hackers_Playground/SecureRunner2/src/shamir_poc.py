from Crypto.Util.number import getPrime, bytes_to_long
from gmpy2 import gcd

p = getPrime(512)
q = getPrime(512)
n = p * q
e = 0x10001
d = pow(e, -1, (p - 1) * (q - 1))
dp = d % (p - 1)
dq = d % (q - 1)
a = q * pow(q, -1, p)
b = p * pow(p, -1, q)

m = bytes_to_long(open("/dev/urandom", "rb").read(48))

def modexp(m):
	r = 0x3928501df6e7e533
	sp = pow(m, d, p * r)
	sq = pow(m, d, q * r)
	
	#sq ^= 0xff << 64		#Fault injection
	assert(sp % r == sq % r)

	CRT = a * sp + b * sq	#CRT

	return CRT % n

print(modexp(m))
print(pow(m, d, n))

