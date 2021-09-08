from Crypto.Util.number import bytes_to_long, GCD
from Crypto.PublicKey import RSA
from util import volatile_DB as DB
import random


class myRSA:
    def __init__(self, keyFile):
        with open(keyFile) as f:
            rsa = RSA.importKey(f.read())
        self._db = DB()

        keydata = ['p', 'q', 'd', 'u', 'n', 'e']
        p, q, d, u, n, e = (getattr(rsa, attr) for attr in keydata)

        self.n = rsa.n
        self.e = rsa.e
        del rsa

        dp = d % (p - 1)
        dq = d % (q - 1)

        self._db.save('p', p)
        self._db.save('q', q)
        self._db.save('dp', dp)
        self._db.save('dq', dq)
        self._db.save('u', u)

    def sign(self, m):
        p, q, u = 1, 1, 1
        while p * q != self.n:
            p = self._db.load('p')
            q = self._db.load('q')

        while u * p % q != 1:
            u = self._db.load('u')

        while True:
            dp = self._db.load('dp')
            dq = self._db.load('dq')

            s1 = pow(m, dp, p)
            s2 = pow(m, dq, q)

            h = s2 - s1
            if h < 0:
                h = h + q

            h = h * u % q
            s = h * p + s1

            # Avoid fault
            if GCD(pow(s, self.e, self.n) - m, self.n) not in [p, q]:
                return s

    def encrypt(self, msg):
        m = bytes_to_long(msg)
        return pow(m, self.e, self.n)


if __name__ == '__main__':
    rsa = myRSA('key.pem')
    with open('flag.txt', 'rb') as f:
        flag = f.read()

    with open('data.txt', 'w') as f:
        f.write('(%d, %d)\n' % (rsa.n, rsa.e))

        for i in range(20):
            m = random.getrandbits(rsa.n.bit_length() - 1)
            s = rsa.sign(m)
            f.write('(%d, %d)\n' % (m, s))

        f.write('%x\n' % (rsa.encrypt(flag)))
