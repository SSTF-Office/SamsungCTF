from Crypto.Util import number
import sys

p = number.getPrime(512)
q = number.getPrime(512)
n = p * q
print "N : {:x}".format(n)

m = number.bytes_to_long('\x01' * (512 / 8))
e = 257
d = number.inverse(e, (p - 1) * (q - 1))
print "len(d) : {:d}".format(d.bit_length())

flag = sys.argv[1] if len(sys.argv) == 2 else "SAMPLE FLAG"
flag_enc = pow(number.bytes_to_long(flag), e, n)
print "enc(flag) : {:x}".format(flag_enc)

WINDOW_SIZE = 8
WINDOWS = [0] * (1 << WINDOW_SIZE)
mm, m2 = m, m * m % n
for i in xrange(1, (1 << WINDOW_SIZE), 2):
    WINDOWS[i] = mm
    mm = mm * m2 % n


def f(x):
    res = 1
    for i in xrange(WINDOW_SIZE):
        if x & (1 << i):
            res = WINDOWS[x >> i]
            break
    return res, i


def fast_exp(x, n, error_iter=-1):
    mask = (1 << WINDOW_SIZE) - 1
    res, xl = 1, []
    while x:
        xl.insert(0, x & mask)
        x >>= WINDOW_SIZE

    u, s = f(xl[0])

    res = res * u % n
    for j in xrange(s):
        res = res * res % n

    if error_iter == 0:
        res ^= 0xff

    for i in xrange(1, len(xl)):
        u, s = f(xl[i])

        for j in xrange(WINDOW_SIZE - s):
            res = res * res % n
        res = res * u % n
        for j in xrange(s):
            res = res * res % n

        if error_iter == i:
            res ^= 0xff
    return res


print "Sign : {:x}".format(fast_exp(d, n))
for i in range(d.bit_length() // WINDOW_SIZE):
    print "Attack on {:2d} : {:x}".format(i, fast_exp(d, n, i))

