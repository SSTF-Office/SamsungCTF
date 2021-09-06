# crashcollector

## Description
Please report all bugs!
(ip : port, binary link, libc link)

## Challenge setup
./deploy/run.sh

## Exploit

### phase 1 : find bug
Program implements simple string tokenizing algorithm.

There is simple bug while parsing the tokens. So we can trigger Crash handler.


### phase 2 : info leak
Inside crash handler, there is another bug.

If the stack situation is ideal, the bug leaks register context to user.

To trigger info-leak, user can controll stack contents and overwrite the CrashHandler stack

with the previously-existing stack contents (with the sprintf inside CrashHandler).

With this stack BOF in crashhandler, attacker can overwrite the FD of send-report feature into 0 (socket)

once, the FD is overwritten into 0, the register context is sent to user regardless of IP/PORT.


### phase 3 : RIP control
after info is leaked. there is another bug inside crash handler

which allows attacker to hijack RIP. To get RIP, we need to overwrite

inner-stack-frame of `scanf`.

cannot overwrite GOT because full-relro



### phase 4 : Full ROP
once RIP is hijacked, we need to extend it to full ROP by jumping into

fgets inside bug fuction. jumping into system won't work because of sandbox(SECCOMP)

anyway, because of the sudden jump, stack frame is messed

so we can overwrite stack of fgets with fgets.

with ROP, we execute mprotect and gets to execute open-read-write shellcode.


