# dvchat

## Description
Damn Vulnerable Chat!
(ip : port, binary link, libc link)

## Challenge setup
./deploy/run.sh

## Exploit

### phase 1 : find bug
This is a skeleton interface for NCURSES based chatting program
There is three threads, two of child threads are SECCOMP sandboxed
There is a thread-race-condition vulnerability that allows stack underflow

### phase 2 : info leak
Using the stack underflow in getinput function, attacker can extend the
bug into full-chain ROP by using scanf. initially (%14$s) overwrites main's
stack and bypass CANARY.

### phase 3 : GOT overwrite
using ROP, attacker leaks LIBC with GOT address
then, trigger second exploit with scanf (replace free - scanf)

### phase 4 : call system
free's GOT is changed to system.
now we jump into free("sh");

