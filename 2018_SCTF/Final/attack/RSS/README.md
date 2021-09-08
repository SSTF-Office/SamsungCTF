# RSS
## Description 
```
Real Secure Storage (RSS)

This Storage is really secure!!! It uses Trusted Execution Environment(TEE).

But.... Are you think that it is really secure?

Level1 flag: /home/rss/flag
Level2 flag: Encryption key

```
# RSS Level 1
## Exploit

### Bug
When add two integer by concat function, There is a type confusion bug.

### Phase1: Infoleak

Using type confusion bug, attacker can leak any address(especially, GOT) by using list function.

### Phase2: GOT overwrite

Using type confusion bug, attacker can overwrite any address by using substring,concat function.
In this case, attacker can overwrite a GOT of free func to system func.

### Phase3: Triggering

Insert string '/bin/sh #' and any integer, and then concat them.
It triggers a system func likes "system('/bin/sh # {any integer}')".

# RSS Level 2
## Exploit

### Bug

Simple BOF in encrypt function. but, attacker can't easily exploit it by distributed binary.
You must make your own binary to exploit Trustzone or.... use HARD ROP.

(Chosen-plain text attack is also intended solution.)
### Phase1: Analysis

Trustzone binary has different structure from normal binary (https://github.com/OP-TEE/optee_os/blob/master/scripts/sign.py)

### Phase2: BOF

When store string to Trustzone, there is no size validation check on Trustzone side.
Using this feature, attacker can overwrite a count of blocks variable.


### Phase3: Leak

Using debug function.

