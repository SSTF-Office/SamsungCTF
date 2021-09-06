import sys
from Crypto.Cipher import AES

BS = 16
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS)
unpad = lambda s : s[0:-ord(s[-1])]

key = b'kingodemperorchungmugongalmighty'
iv = b'superduperinjung'
cipher = AES.new(key, AES.MODE_CBC, iv)

if __name__ == '__main__':
    pt = sys.stdin.read()
    ct = cipher.encrypt(pad(pt))
    sys.stdout.write(ct)

