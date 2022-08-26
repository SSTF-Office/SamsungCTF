## SecureRunner2

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Medium-Hard         |
| Category          | Crypto, Pwnable     |
| Tags              | RSA, Fault Injection|
| Author            | matta               |

### Description
 > It's a PATCHED white-list based command executor.

### Deployment
 * Download: deploy/download/securerunner2.zip
 * Server: docker-compuse up -d --build (in deploy/server)

### Solving strategy

A challenge RSA Modulus Falut Injection into RSA CRT.

The challenge implementation is almost the same as `SecureRunner`, but a mitigation about RSA Falut Injection is added.

Now we cannot corrupt p or q value.

To solve the challenge, we should corrupt the modulus, `n` with the same manner of the `SecureRunner` exploit.

Before corrupting `n`, we should generate normal signatures for 6 commands.

And generate corrupted signatures with modified `n`.

Now that we have 6 normal/abnormal signature pairs, factorizing `n` is possible by using Modulus Fault Attack based on LLL.

Reference:
 - Modulus Fault Attacks Against RSA-CRT Signatures([talk](https://www.normalesup.org/~tibouchi/papers/talk-modulusfault.pdf), [paper](https://eprint.iacr.org/2011/388.pdf))
