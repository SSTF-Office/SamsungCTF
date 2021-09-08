# polyPKC

## Description

```
This PKC has serious (and easy to find) vulnerability in its design.

Can you analyze and find it?
```

## How to run

give `problem.py` and `enc` to challengers.

## Exploit

Public key `f` satisfies `f(p) = 0`. Then, f = (x-p)g with polynomial g.

Encryption is done by `f * r % (x^order -1) + m`, and `p^order - 1 = 0`.

If m = 0, then every encrypted polynomial has (x-p) as divisor. 

Then, get ENC(0,f) several times and their gcd eventually goes to (x-p).

```python
secret = poly_gcd(poly_enc(pub_poly, 0, N), poly_enc(pub_poly, 0, N), N)
key = N - (secret[0] * modinv(secret[1], N) % N)
# and decrypt with the key
```
