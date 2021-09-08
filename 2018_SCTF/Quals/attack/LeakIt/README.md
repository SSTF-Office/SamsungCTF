# LeakIt

## Description
This is a communication program that all the communications are encrypted with GPG key. Therefore, you can communicate with the admin. Can you decrypt the secret flag file sent to the admin?

leakit.eatpwnnosleep.com:42424, [binary](#)

## FLAG
SCTF{b4by_PwN_w1th_crit1Cal_memOry_l3ak}

## Challenge setup
NOTE that this will add script 'copybot.sh' in the 'deploy' directory to your
crontab. Please check before you execute this.
```/bin/sh
$ ./run.sh
```

## Exploit
There are two solutions AFAWK in the leaking stage.

### phase 1 : Bypass Canary
When you send msg, there happens a simple BOF. Because the server forks, you can
bruteforce the canary.

### phase 2 : Arbitrary code execution
After bypassing canary, you can simply construct an ROP chain to execute any
command, which includes reading arbitrary stack. Here, I introduce possible two
ways to execute arbitrary code. In fact, there are two vulnerabilities AFAIK.

#### phase 2-1: ROP Chain
Because you already know the canary, you can overwrite the return address.
However, because you do not know the image base where current ELF is loaded, you
have to identify this first. You can leak each byte of the return address just
same as leaking the canary, which tells you the image base. With this
information, you can now jump to any address in the current binary.

#### phase 2-2: Command injection
In addition, there is a command injection vulnerability when receiving a message
from a user, which you can execute any arbitrary shell command. Note that
'/proc' is unmounted so you cannot execute commands accessing '/proc'


### phase 3: Leak password
Because the admin's GPG key id and passphrase are in the stack, you should read
this to decrypt to encrypted flag file. You can leak password in various ways.
Here, I introduce two possible ways.

#### phase 3-1: Leaking password using SFP
Because you already know the canary, you can modify the SFP located just before
the return address. By modifying lower bytes of SFP, you can leak the id and
password of the admin from the stack.

#### phase 3-2: Leaking password using environ
Because you already know the canary, you can leak any address by chaining 'puts'
function. Using that, you can leak libc address and identify the libc in the
libc database using the offset diff of two libc functions. Then, you can search
the environ in the libc (because environment variables are used in the main
function). Now you know the stack address and scan it to get the password.

### phase 3 : ROP chain to execute decrypt command
Finally, you construct an ROP chain to execute arbitrary shell command. You need
to execute GPG decrypt command to get the flag. (the decryption command is
already in the admin's menu. What a kind menu!?
