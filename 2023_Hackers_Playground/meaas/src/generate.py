import json
from types import SimpleNamespace
from Crypto.Util.number import getPrime

rsa_keysize = 1024

rsa = SimpleNamespace()
rsa.p = getPrime(rsa_keysize // 2)
rsa.q = getPrime(rsa_keysize // 2)
rsa.e = getPrime(rsa_keysize // 2)
rsa.n = rsa.p * rsa.q
rsa.d = pow(rsa.e, -1, (rsa.p - 1) * (rsa.q - 1))

with open("rsaparams", "w") as f:
	f.write(json.dumps(rsa.__dict__))
