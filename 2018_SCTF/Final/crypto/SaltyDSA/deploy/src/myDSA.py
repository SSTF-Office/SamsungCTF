from Cryptodome.Util.number import (long_to_bytes as l2b,
                                    bytes_to_long as b2l)
from Cryptodome.Math.Numbers import Integer
from Cryptodome.Util.asn1 import DerSequence
from Cryptodome.PublicKey import DSA
from Cryptodome.IO import PEM
from hashlib import md5

__all__ = ['myDSA', 'import_key']


class myDSA(DSA.DsaKey):
    def __init__(self, key_dict, salt=None):
        super().__init__(key_dict)
        if self.has_private():
            if salt is None:
                raise ValueError('Salt should be given for private key')

            blind = md5(l2b(self.x)).digest()
        else:
            blind = None

        self.pre_salt = salt
        self.post_salt = blind

    def sign(self, msg):
        if not self.has_private():
            raise TypeError('Cannot sign with public key')

        k = b2l(md5(self.pre_salt + msg + self.post_salt).digest())
        m = b2l(md5(msg).digest())

        return tuple(self._sign(m, k))

    def verify(self, msg, sign):
        m = b2l(md5(msg).digest())
        return self._verify(m, sign)

    def export_key(self, keytype):
        ints = [self.p, self.q, self.g, self.y]

        if keytype == 'priv':
            ints.append(self.x)
            key_type = 'myDSA PRIVATE KEY'

        elif keytype == 'pub':
            key_type = 'myDSA PUBLIC KEY'

        else:
            raise ValueError('keytype should be one of "pub" or "priv"')

        binary = DerSequence(ints).encode()
        return PEM.encode(binary, key_type)

    exportKey = export_key


def import_key(extern_key, salt=None):
    der, marker, _ = PEM.decode(extern_key)
    ints = DerSequence().decode(der)

    if not marker.startswith('myDSA'):
        raise ValueError('Invalid format')

    key_dict = dict(zip(('p', 'q', 'g', 'y', 'x'), map(Integer, ints)))
    return myDSA(key_dict, salt)


importKey = import_key
