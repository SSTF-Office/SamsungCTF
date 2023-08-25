# CTF challenge for SSTF2023

This repository contains CTF (Capture The Flag) challenge for Samsung Security Tech. Forum (SSTF2023).

_Note:_ use at least Python 3.8 to run the code in this repository
## Idea

CTF challenge is based on RSA attack 2022 described in 
[Chuengsatiansup, C., Feutrill, A., Sim, R.Q., Yarom, Y. (2022). _RSA Key Recovery from Digit Equivalence Information_](https://doi.org/10.1007/978-3-031-09234-3_10) which in turn is 
an extension of attack 2009 described in [Heninger, N., Shacham, H. (2009). _Reconstructing RSA Private Keys from Random Key Bits_](https://doi.org/10.1007/978-3-642-03356-8_1).

RSA implemented via Chinese Reminder Theorem looks like this:
```python
flag = "FLAG"
p = 2011123
q = 2011127
n = p * q
phi_n = (p-1) * (q-1)
e = 2 ** 16 + 1
d = mod_inverse(e, phi_n)

# pre-compute d_p, d_q, q_inv
d_p = d % (p-1)
d_q = d % (q-1)
q_inv = mod_inverse(q, p)

# RSA exponentiation (encrypt / decrypt)
def rsaChineseReminderTheorem(text: int, p: int, q: int, d_p: int, d_q: int, q_inv: int):

    m_p = pow_mod(text, d_p, p) # leaks memory access pattern
    m_q = pow_mod(text, d_q, q) # leaks memory access pattern
    h   = q_inv * (m_p - m_q) % p
    return m_q + h * q
```
Chinese Reminder Theorem allows to encrypt/decrypt faster. For example:
```python
# encrypt - via pow()
flag_as_int = string_to_int(flag)
ciphertext = pow(flag_as_int, e, n)
print("encrypted flag:", ciphertext)

# decrypt - via Chinese Reminder Theorem
print("memory access pattern leaked during decryption:")
flag_as_int_ = rsaChineseReminderTheorem(ciphertext, p, q, d_p, d_q, q_inv)
flag_ = int_to_string(flag_as_int_)

assert flag_as_int == flag_as_int_
print("Flag:", flag_)
```
It uses two exponentiations which are assumed to leak data:
```python
    m_p = pow_mod(text, d_p, p) # leaks memory access pattern
    m_q = pow_mod(text, d_q, q) # leaks memory access pattern
```
Specifically if `pow_mod()` is implemented as Fixed Window Exponentiation 
(described in Algorithm 1 of 2022 paper), then 
access pattern of reading pre-computed digits can be sniffed from the cache.
![[]](fixwinexp.png)

RSA attack to be used to solve CTF challenge assumes:
- re-use of OpenSource implementation of 2009 attack ([example](https://github.com/jvdsn/crypto-attacks/blob/master/attacks/factorization/branch_and_prune.py))
- create its custom extension for 2022 attack



## References

- [Bad Keys](https://badkeys.info) can be useful to make sanity check of the keys (after [converting them to *.pem](https://stackoverflow.com/questions/10367072/m2crypto-import-keys-from-non-standard-file))