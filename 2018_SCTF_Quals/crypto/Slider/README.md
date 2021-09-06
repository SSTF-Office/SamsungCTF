# Slider

## Description

```
I heard the AES can be used as a safe permutation so my slider should be safe.

nc slider.eatpwnnosleep.com 6884
```

[link](#)

## How to run
Use `run.sh` in deploy directory to run docker script
or simply run `python3 slider.py`

## flag
`SCTF{Did_y0u_3nj0y_my_5lid3r?}`

## How to solve
Note that given cipher has four round periocity so we can consider slide attack.
```
Dec : F2 - F0 - F1 - F0 - F2 - ... - F0 - F1 - F0            (1)
Enc :      F0 - F1 - F0 - F2 - ... - F0 - F1 - F0 - F2       (2)
Dec :           F2 - F0 - F1 - ... - F0 - f2 - F0 - F1 - F0  (3)
```
We can find k2 from (1) and (2) with 256 encryption and decryption by classic slide attack.
Also we can find k0 and k1 ^ k2 from (2) and (3) with 256 encryption and decryption
by combining complementation slide and sliding with a twist. You can see the detail at
section 3.3 in [link](https://www.iacr.org/archive/eurocrypt2000/1807/18070595-new.pdf)
