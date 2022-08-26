## SecureRunner

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Easy                |
| Category          | Crypto, Pwnable     |
| Tags              | RSA, Fault Injection|
| Author            | matta               |

### Description
 > It's a white-list based command executor.

### Deployment
 * Download: deploy/download/securerunner.zip
 * Server: docker-compuse up -d --build (in deploy/server)

### Solving strategy

A challenge about Fault Injection into the RSA CRT.

The binary generates an RSA signature for pre-defined commands, or execute a command with valid signature.

You can simply find a hidden dish - 0 and 9999, and 9999 can be executed only once with 4 bytes FSB vulnerability.

You use 9999 to clear some word in the heap while RSA-CRT parameters are in the heap.

By several attempts - simple brute forcing - you can corrupt p or q value using this vulnerablity.

 * As `heap + index` value is at the stack, so you can use `%7$n` directive.
 * You can check original n value from 0 or 2 first.
 * Try to corrupt p or q varing the index
 * If you succeeded in corrupting it, then you can check it by menu 0 which calculates p \* q.

p and q values are used to calculate CRT while signature generation, no you can trigger RSA Fault Injection attack.

After you get the signature `s` from corrupted values, then you can find p from `p = gcd(pow(s, e) - m, n)`.

Reference:
 - The Boneh-DeMillo-Lipton fault attack (1997)
