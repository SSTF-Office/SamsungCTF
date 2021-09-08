# Tracer

## Description

```
I made a very secure encryption program.
I encrypted my secret using this program,
but I forgot the key for decrypting...

Please recover my secret.

(tar.gz download link)
```

## How to run
No service to run

## How to solve
0. This is a program implementing EEECC.
1. We will recover the plaintext through calculating c - (r * priv_key). Since r and c is already given, extracting the private key is the only challenge.
2. Recover private key from multiplying routine. Branch instruction at a multiplying routine shows each bit of private key.
3. Calcuate plaintext. c - (r * priv_key) will give you a flag

## FLAG
```
SCTF{Ev3r_get_th4t_feelin9_of_dejavu_L0LOL}
```
