# Report

## Description
Create grade report for student.

## Comment

Check this. [COOP, S&P'15] (http://syssec.rub.de/media/emma/veroeffentlichungen/2015/03/28/COOP-Oakland15.pdf)

1. First We can Leak the Heap Address, because there is no NULL byte at the end of teacher name

2. The vulnerable code is very easy to see. (Just Simiple Buffer Overflow)

3. But some check code for virtual function call.
 - We can not change the virtual function pointer to fake function. Only use the exist virtual function for exploit

4. Thus we got the shell in limited environment using COOP attack
 - Make Chain of virtual functions and overwrite function pointer in libc (like free_hook)

>**This problem is dependent on libc version**

>**I test full exploit in Ubuntu 16.04 with libc-2.23.so**

>**It's remote exploit problem. using `xinetd` or `socat`

## How to Run
In deploy folder
```/bin/sh
$ docker build -t report .
$ docker run -d -p 55555:55555 report
```

## Requirements
python3


