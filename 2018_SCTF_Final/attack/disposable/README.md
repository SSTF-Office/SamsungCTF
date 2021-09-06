# Disposable

## Description
What was that "disposable stack" you were talking about?

`nc disposable.eatpwnnosleep.com 30010`

[disposable.zip](https://s3.ap-northeast-2.amazonaws.com/sctf2018-final-binaries/disposable.zip_05a49b90abc6b1d46b794d160be3404ad1538d35)

## How to run

Run inside `deploy` directory:
```
# ./run.sh
```

## How to solve

1. Figure out the semantics of new function prologue/epilogue. Every time a function is invoked, the following operations are performed:
   - at prologue, it subtracts fs:0x158 from stack pointer
   - at epilogue, it adds its size of stack frame to fs:0x158.

   This will prevent a stack variable from being reused at the same address.
   However, the size of fs:0x158 cannot exceed 0xFFFF.
2. Find the vulnerability in `sub_400896()` where the second argument is left
   uninitialized if given user name is not found.
3. Spray the stack with pointers &mdash;
   `printf@got - 128`, `printf@got - 128 + 4`, and `memcpy@got - 128 - 4` &mdash;
   to (a) leak the address of `printf@libc`, and (b) overwrite `memcpy@got` with `system@libc`.
4. Send two messages with their contents being `system@libc` and `/bin/sh`, respectively.
