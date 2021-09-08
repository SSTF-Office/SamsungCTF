# Volatile

## Description

I made my own RSA signer using database. Later I noticed the database
did not work properly, but I believe the signer is safe from errors.
[link](#)

## How to run
No service. Only `data.txt` and `rsa.py` are given. Run `python3 rsa.py`
to generate data. Note that there exists very small probability that it
generates invalid(unsolvable) data. Therefore, one should check validity
by running exploit code before deploy.

## flag
`SCTF{Claaaasic_RSA_challenge_from_errors}`

## How to solve
In this challenge, we combined two well known fault attacks on RSA: fault
in exponents and CRT. In `rsa.py`, other private variables except `dp` and `dq`
are checked properly before using so that they are safe from faults. For
`dp` and `dq`, I checked only for `GCD(s^e - m, n) in [p, q]` which avoids
classic fault attack on CRT part.

The problem occurs when `GCD(s^e - m, n) = 1`, which tell us there were faults
on both `dp` and `dq`. Here we can use the idea of fault attack on exponents.
First, we guess error occured in a bit (so it flipped). If i'th bit was
flipped, one of `s * m^i` or `s * m^-i` will be valid sign in
`mod p` or `mod q`. Then we can apply fault attack on CRT to find `p` or `q`.

You may see the details for attacking fault in exponents at writeups of
Radioactive(pctf 2016) or BrokenBox(CSAW 2016). For the fault attack
on CRT, see section 5.2 in [link](https://crypto.stanford.edu/~dabo/papers/RSA-survey.pdf).
