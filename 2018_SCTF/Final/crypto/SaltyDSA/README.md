# SaltyDSA

## Description

Here are simple login servers with custom DSA using salt.
Unfortunately, the private key of server\_1 was leaked
but I still believe server\_2 is secure.
Can you collect the secret files and get the flag?

Note : Two servers share the salt file.

## How to run
Server1: run with hacked private keys

Server2: run with normal private keys

See detail in `start.sh`

## How to solve
1. Using known private key of server1, find a value `md5_block(pad(salt || msg))`
called `ihv`.

2. Find a collision from given `ihv` with [fastcoll](https://github.com/upbit/clone-fastcoll).
(md5 chosen prefix collision)

3. From collision, make two sign with identical `k` and recover `k` from server2.

4. From `k`, recover `x` and generate forgery on server2.

Following links will help you.
[link1](https://rdist.root.org/2010/11/19/dsa-requirements-for-random-k-value)
[link2](https://rdist.root.org/2009/05/17/the-debian-pgp-disaster-that-almost-was/)


## Flag
`SCTF{You_really_have_to_generate_k_in_DSA_carefully}`
