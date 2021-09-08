from functools import reduce
from Crypto.Hash import SHA256
from Crypto.Util.number import getPrime, getRandomRange, isPrime
from Crypto.Math.Numbers import Integer
from myDSA import myDSA
import itertools


def gen_smooth(bits, s):
    while True:
        r = getPrime(s)
        q = getPrime(18)
        R = [getPrime(36), q, q]
        t = s + 1
        for i in range(0, t):
            R.append(r // 8 + getRandomRange(0, s ** 2))
        p = reduce(lambda a, b: a * b, R, 2)
        print(p.bit_length())
        if p.bit_length() == bits:
            return p


def gen_dsa_from_q(bits, q):
    q = Integer(q)

    # Generate p (A.1.1.2)
    L = bits
    outlen = SHA256.digest_size * 8
    n = (L + outlen - 1) // outlen - 1
    b = L - 1 - (n * outlen)

    seed = b'Crypto'

    offset = 1
    upper_bit = 1 << (L - 1)
    while True:
        V = [SHA256.new(seed + Integer(offset + j).to_bytes()).digest()
             for j in range(n + 1)]
        V = [Integer.from_bytes(v) for v in V]
        W = sum([V[i] * (1 << (i * outlen)) for i in range(n)],
                (V[n] & ((1 << b) - 1)) * (1 << (n * outlen)))

        X = Integer(W + upper_bit)  # 2^{L-1} < X < 2^{L}
        assert(X.size_in_bits() == L)

        c = X % (q * 2)
        p = X - (c - 1)  # 2q divides (p-1)
        if p.size_in_bits() == L and isPrime(int(p)):
            break
        offset += n + 1

    # Generate g (A.2.3, index=1)
    e = (p - 1) // q
    for count in itertools.count(1):
        U = seed + b"ggen" + bytes([1]) + Integer(count).to_bytes()
        W = Integer.from_bytes(SHA256.new(U).digest())
        g = pow(W, e, p)
        if g != 1:
            break

    return int(p), int(g)


def paramGen_old():
    with open('/dev/urandom', 'rb') as f:
        salt = f.read(12)

    with open('./salt.bin', 'wb') as f:
        f.write(salt)

    # Invalid DSA with gen_smooth_prime(128, 8)
    q = Integer(gen_smooth(128, 8))
    p, g = map(Integer, gen_dsa_from_q(768, q))

    c = Integer.random(exact_bits=q.size_in_bits() + 64)
    x = c % (q - 1) + 1
    y = pow(g, x, p)

    key_dict = {'y': y, 'g': g, 'p': p, 'q': q, 'x': x}
    dsa = myDSA(key_dict, salt)

    with open('./vuln_pub.pem', 'w') as f:
        f.write(dsa.export_key('pub'))

    with open('./vuln_priv.pem', 'w') as f:
        f.write(dsa.export_key('priv'))

    # Valid DSA
    q = Integer(getPrime(128))
    p, g = map(Integer, gen_dsa_from_q(768, q))

    c = Integer.random(exact_bits=q.size_in_bits() + 64)
    x = c % (q - 1) + 1
    y = pow(g, x, p)

    key_dict = {'y': y, 'g': g, 'p': p, 'q': q, 'x': x}
    dsa = myDSA(key_dict, salt)

    with open('./pub.pem', 'w') as f:
        f.write(dsa.export_key('pub'))

    with open('./priv.pem', 'w') as f:
        f.write(dsa.export_key('priv'))


def paramGen_new():
    with open('/dev/urandom', 'rb') as f:
        salt = f.read(12)

    with open('./salt.bin', 'wb') as f:
        f.write(salt)

    for i in [1, 2]:
        # Valid DSA
        q = Integer(getPrime(128))
        p, g = map(Integer, gen_dsa_from_q(768, q))

        c = Integer.random(exact_bits=q.size_in_bits() + 64)
        x = c % (q - 1) + 1
        y = pow(g, x, p)

        key_dict = {'y': y, 'g': g, 'p': p, 'q': q, 'x': x}
        dsa = myDSA(key_dict, salt)

        with open('./server_%d_pub.pem' % (i,), 'w') as f:
            f.write(dsa.export_key('pub'))

        with open('./server_%d_priv.pem' % (i,), 'w') as f:
            f.write(dsa.export_key('priv'))


if __name__ == '__main__':
    paramGen_new()
