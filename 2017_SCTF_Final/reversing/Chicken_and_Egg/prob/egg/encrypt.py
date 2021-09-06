import sys
from Crypto.Cipher import AES

key = b'awesomecipherkey'
iv = b'handsomeinitvect'
cipher = AES.new(key, AES.MODE_CBC, iv)

m = sys.stdin.read()
padsize = (16 - (len(m) % 16))
m += chr(padsize) * padsize
c = cipher.encrypt(m)
sys.stdout.write(c)
