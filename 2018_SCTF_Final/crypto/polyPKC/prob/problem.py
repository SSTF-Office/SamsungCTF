from Crypto.Util import number

class PubKey(object):

    def __init__(self, poly, order, n):
        self.poly = poly
        self.order = order
        self.N = n

    def poly_rand(self):
        return [number.getRandomRange(0, self.N) for i in range(0, self.order)]

    def poly_mul(self, p, q):
        res = [0 for _ in range(self.order)]
        for i in range(self.order):
            for j in range(self.order):
                k = (i + j) % self.order
                res[k] = (res[k] + (p[i] * q[j])) % self.N
        return res

    def encrypt(self, m):
        x = self.poly_mul(self.poly, self.poly_rand())
        x[0] = (x[0] + m) % self.N
        return x


class PrivKey(object):

    def __init__(self, t, N):
        self.t = t
        self.N = N

    def poly_apply(self, p, x):
        res = 0
        for i in range(len(p)):
            res += p[i] * pow(x,i,self.N)
        return res % self.N


    def decrypt(self, c):
        return self.poly_apply(c, self.t)

class KeyGenerator(object):

    def __init__(self, order, bits):
        q = 2
        p = 2
        N = p * q
        r = number.getRandomRange(1, N)
        t = pow(r, (p-1) * (q-1) / order, N)
        while (pow(t,order,N) != 1) or t == 1 or t == N -1:
            p = number.getPrime(bits)
            q = number.getPrime(bits)
            N = p * q
            r = number.getRandomRange(1, N)
            t = pow(r, (p-1) * (q-1) / order, N)

        pub_poly = [number.getRandomRange(0, N) for i in range(0, order)]

        fin = 0
        for i in range(1,order):
            fin += pub_poly[i] * pow(t, i, N) % N
            fin %= N
        pub_poly[0] = N - fin

        self.pubKey = PubKey(pub_poly, order, N)
        self.privKey = PrivKey(t, N)

key = KeyGenerator( 51, 512 )

with open('flag','r') as fp:
    m = int(fp.read().encode('hex'), 16)
with open('enc','w') as fp:
    fp.write('order : %d\n' % key.pubKey.order)
    fp.write('public key : %s\n' % str(key.pubKey.poly))
    fp.write('N : %d\n' % key.pubKey.N)
    fp.write('encrypted message : %s\n' % str(key.pubKey.encrypt(m)))
