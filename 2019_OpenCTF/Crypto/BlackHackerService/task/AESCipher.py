import base64
from hashlib import sha256
from Crypto import Random
from Crypto.Cipher import AES

class AESCipher(object):
    def __init__(self,key):
        self.block_size = AES.block_size
        self.key = sha256(key).digest()
        self.iv = "A"*16 #"Random.new().read(self.block_size)"

    def encrypt(self,plaintext):
        plaintext = self._pad(plaintext)
        cipher = AES.new(self.key,AES.MODE_CBC,self.iv)
        return base64.b64encode(cipher.encrypt(plaintext))

    def decrypt(self,ciphertext):
        ciphertext = base64.b64decode(ciphertext)
        cipher = AES.new(self.key,AES.MODE_CBC,self.iv)
        return self._unpad(cipher.decrypt(ciphertext))

    def _pad(self,s):
        l = len(s)
        p = self.block_size - (l % self.block_size)
        return s + p * chr(p)

    @staticmethod
    def _unpad(s):
        pad = s[-1]
        for x in s[len(s)-ord(pad):]:
            if x != pad: return None
        return s[:len(s)-ord(pad)]
