from Crypto.PublicKey import RSA
import random


class volatile_DB:
    def __init__(self):
        self.db = dict()

    def save(self, nid, n):
        self.db[nid] = n

    def load(self, nid):
        n = self.db[nid]
        error = random.choice([True, False, False])

        if error:
            error_ind = random.randint(0, n.bit_length() - 1)
            n ^= 1 << error_ind

        return n


def paramGen():
    rsa = RSA.generate(2048)
    with open('key.pem', 'wb') as f:
        f.write(rsa.exportKey())


if __name__ == '__main__':
    paramGen()
