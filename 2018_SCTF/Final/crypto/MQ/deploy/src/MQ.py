from random import randint
from binascii import hexlify
import sys


def wline(msg=''):
    sys.stdout.write(msg + '\n')
    sys.stdout.flush()


class MQpoly:
    def __init__(self, terms, p, n):
        self.terms = terms
        self.p = p
        self.n = n

    def apply(self, vals):
        if len(vals) != self.n:
            raise ValueError('Values for n variables should be given')

        quad, uni, c = self.terms

        ret = 0
        for i in range(self.n):
            for j in range(i, self.n):
                ret += quad[i][j] * vals[i] * vals[j]
                ret %= self.p

        for i in range(self.n):
            ret += uni[i] * vals[i]
            ret %= self.p

        ret += c
        ret %= self.p

        return ret

    def __str__(self):
        quad, uni, c = self.terms

        st_terms = []
        for i in range(self.n):
            for j in range(i, self.n):
                v = quad[i][j]
                if v != 0:
                    st_terms.append('%dx%dx%d' % (v, i+1, j+1))

        for i in range(self.n):
            v = uni[i]
            if v != 0:
                st_terms.append('%dx%d' % (v, i+1))

        if c != 0:
            st_terms.append('%d' % (c,))

        return ' + '.join(st_terms)

    @classmethod
    def random_element(cls, p, n):
        quad = [[randint(0, p-1) if j <= i else None for j in range(n)]
                for i in range(n)]

        for i in range(n):
            for j in range(i+1, n):
                quad[i][j] = quad[j][i]

        uni = [randint(0, p-1) for i in range(n)]
        c = randint(0, p-1)

        terms = (quad, uni, c)
        return cls(terms, p, n)


if __name__ == '__main__':
    p = 131
    n = 32

    with open('flag.txt', 'rb') as f:
        flg = f.read()

    with open('/dev/urandom', 'rb') as f:
        flg += f.read(n - len(flg))

    mq = MQpoly.random_element(p, n)
    wline(str(mq))
    for i in range(n+1):
        lst_1 = sys.stdin.buffer.read(n)
        lst_2 = [(b1 + b2) % p for b1, b2 in zip(lst_1, flg)]
        clue = [mq.apply(lst_1), mq.apply(lst_2)]

        wline(hexlify(bytes(clue)).decode())

