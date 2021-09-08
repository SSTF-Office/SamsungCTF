# shellfuzz

## Description
Lets fuzz the system calls!
(ip : port, source code)

## Challenge setup
./deploy/run.sh

## Exploit

### phase 1 : ulimit -n
This is setuid challenge. so player get's unprivileged shell.
also, player access the binary from directory (e.g., /home/82thf7eef/shellfuzz)
therefore, after getting shell, he can't mess with other players files.

anyway, attacker can raise the 'open fd' to large number with ulimit -n
fuzzer selects parameter between INT and POINTER.
POINTER points random page inside 0xc3000 ~ 0xc4000.
this value is LOWER than ulimit -n's maximum. so we controll our FD.

### phase 2 : dup source file
Fuzzer uses world-redable source file. Attacker can open this and DUP
more than 0xc4000

### phase 3 : using fchmod syscall
using 'fchmod' syscall, player can put +w or SETUID to the fd.
so what we can do here is DUP2 the source file (shellfuzz.c)
as much as we can, and execve to the fuzzer and hope the fuzzer
puts +w and SETUID to this file (this is likely with 1000~2000 attempts)
so we can escalate the privilege with high chance (in few minuets)

once fuzzer makes the shellfuzz.c writable, we replace it with python binary
later, once fuzzer makes the shellfuzz.c setuid/executable we get high privilege shell!!


