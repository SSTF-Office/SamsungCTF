# Messenger

## Description
I made new Messenger with Identity Based Cryptography.

The flag is in admin's messages. Can you read it?

[IBM.py](#) - (logging 부분 제외하고 소스 공개)

## Comment
Server (pk, sk) = (n, g) with RSA public key n

Client (pk, sk) = h(id), g^h(id) % n

Key agreement btw id1, id2 :
sk1 ^ pk2 % n = sk2 ^ pk1 % n = g ^ (pk1 * pk2) % n

(1, a, b) = egcd(h(id1), h(id2)) then g = sk1 ^ a * sk2 ^ b % n

