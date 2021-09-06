# asm3

## Description
run shellcode inside custom sandboxed server
(ip:port, source code link)

## Comment
you can run x86 shellcode which do not exceed 30 bytes.
challenge 1: your shellcode is R_X (not RWX)
challenge 2: you cannot use x86 system call instruction ([int 0x80], [sysenter])
challenge 3: ESP is 0, when your shellcode is executed. you need to pivot stack.
challenge 4: you need to solve SHA1 challenge before testing your shellcode. so no brute-force.

note 1: only two files should be given [asm3.c, wrapper.py] (no binary)
note 2: this is remote challenge. player cannot have access to server(e.g., libc).
note 3: port number of the challenge is configured in [start.pl]

## Challnge setup
1. server must use 64bit OS (tested in ubuntu 16.04 x64)
2. use docker to run the service

## Exploit

### phase 1 : you can pivot stack with VDSO
### phase 2 : you can change CPU mode into 64bit
### phase 3 : after 2, you can use x64 syscall instruction
### phase 4 : you can optimize shellcode length with jmp-call style coding

