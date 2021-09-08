# Chicken and Egg

## Description

```
A month ago, I've used this Chicken app to encrypt my important document.
Then they updated the app two weeks ago.
Now I have to pay 9,999,999 won to decrypt the encrypted file.
What should I do.. plz help me TT

[Download link to Chicken.apk]
[Download link to flag.enc]
```

## Comments

A packed and obfuscated Android application.

The inner code 'Egg' is stored as `Chicken.apk/assets/eggyolk` in encrypted dex form.
When the app starts, the eggyolk is loaded in the JNI library `libegg.so`.
In the library, the code is AES128CBC-decrypted with hard-coded key and iv.
Then it is loaded using a undocumented native API of libdvm.so.

The `libegg.so` is obfuscated using LLVM obfuscator and string obfuscation.
The loaded Egg class contains very simple AES encryption with again, fixed key.

## Deploy

Let players download `Chicken.apk` and `flag.enc`.

## Exploit

- `pip install pycrypto`
- Run `python dec_flag.py <flag.enc >flag.pdf`

## How to change flag

- `pip install pycrypto`
- Change `flag.pdf` (Any file type is ok.)
- run `python enc_flag.py <flag.pdf >flag.enc`

