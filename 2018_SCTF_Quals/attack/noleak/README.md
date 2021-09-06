# noleak

## Description
Please info-leak me!
(ip : port, binary link)

## Challenge setup
./deploy/run.sh

## Exploit

### phase 1 : find bug
There are 3 menus.

menu1 gives heap leak by leaking uninitialized bytes. but this is useless..

In menu3, there is stack bof. with 64bit canary

while BOF, attacker can overwrite idx. so BOF can jump and bypass canary

while doing this, attacker needs to prepare proper heap layout

menu1 is used in here.

menu2 is dummy function as a ruse.

there is system function in binary but there is no 'sh' argument. 

and there is no leak. this is the challenging part.




### phase 2 : start ROP
proper heap layout and idx control can bypass canary and start ROP

system is at plt, however there is no 'sh' argument.

to find this, attacker needs to leak libc. but this is impossible (stdout,err is closed)

solution is to use 'ed' (which exists in non-pie base) and do command injection there.

alternatively, seed value is stored in bss. attacker can 16-bit brute to use 'sh'


### phase 3 : get shell
inside 'ed', user can execute !/bin/sh 1>&0

and get shell. (this only works when binary is xinet.d so fd0 is socket)

