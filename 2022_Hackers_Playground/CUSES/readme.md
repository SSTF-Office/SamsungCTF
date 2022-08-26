## Cuses

### Overview

| Type               | Contents               |
|:------------------:|:-----------------------|
| Difficulty         | Easy                   |
| Category           | Crypto                 |
| Tags               | Web, Crypto            |
| Author             | matta                  |


### Description

> I heared that cookie is obsolete and weak.
>
> So I made a CUstom SESsion using AES encryption.
>
> I am safe now.
>
> http://site.url

### Depolyment

  * Server: deploy/deploy.sh
    flag is in the src/secret.php

### Solving strategy

This is a web based crypto challenge.
Basic idea is similar to `RC four` challenge.

Cookie is constructed as: iv || AES_CTR(username || server_secret)

A challenger can login via guest account only.

So he can know partial plain text and full cipher text

Challenger can make a encrypted cookie of the form `(admin || server_secret)`
by XORing encrypted session data, 'guest', and 'admin'.
