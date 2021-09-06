from itertools import chain, repeat
from Crypto.Cipher import AES
from os import urandom
import signal
import sys


def wline(msg=''):
    sys.stdout.write(msg + '\n')
    sys.stdout.flush()


def rline(msg=None):
    if msg is not None:
        wline(msg)
    return sys.stdin.readline().strip()


def bxor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


def ncycles(iterable, n):
    "Returns the sequence elements n times"
    return chain.from_iterable(repeat(tuple(iterable), n))


class Slider:
    def __init__(self, key):
        self.k0 = key[0:2]
        self.k1 = key[2:4]
        self.k2 = key[4:6]

        self.P = AES.new(bytes(range(16)), AES.MODE_ECB).encrypt

    def _crypt(self, m, k):
        if len(m) != 4:
            return None

        l, r = m[0:2], m[2:4]

        def f(x):
            return self.P(x + bytes(14))[:2]

        for rkey in ncycles(k, 4):
            l, r = r, bxor(l, f(bxor(r, rkey)))

        return r + l

    def encrypt(self, m):
        k = (self.k0, self.k1, self.k0, self.k2)
        return self._crypt(m, k)

    def decrypt(self, m):
        k = (self.k2, self.k0, self.k1, self.k0)
        return self._crypt(m, k)

    def guess(self, k):
        return k == self.k0 + self.k1 + self.k2


class SliderTester:
    def __init__(self):
        key = urandom(6)
        self.slider = Slider(key)

    def handle_crypt(self, cmd, msg):
        if cmd == 'enc':
            res = self.slider.encrypt(msg)
        elif cmd == 'dec':
            res = self.slider.decrypt(msg)
        else:
            res = None

        if res is not None:
            wline(res.hex())
            return True
        else:
            return False

    def handle_guess(self, cand):
        if self.slider.guess(cand):
            try:
                with open('flag.txt') as f:
                    wline(f.read())
            except Exception:
                wline('Call admin plz')
                exit()

    def handle(self):
        try:
            wline('Commands: [enc | dec | guess] <hxstr>')
            for i in range(4 * 2**8 + 1):
                cmd, val = rline().split(maxsplit=2)
                msg = bytes.fromhex(val)
                if cmd in ['enc', 'dec'] and self.handle_crypt(cmd, msg):
                    continue
                elif cmd == 'guess':
                    self.handle_guess(msg)
                break

        except Exception:
            exit()


if __name__ == '__main__':
    server = SliderTester()
    signal.alarm(300)
    server.handle()
