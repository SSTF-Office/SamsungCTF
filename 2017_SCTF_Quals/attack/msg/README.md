# msg

## Description
Simple message server

(ip:port, binary link)


## Comment
User can write, read, change messages.

Messages are stored as each file in "/home/msg/db/".


## Exploit

### phase 1 : Close stderr
With BOF, attacker can control fd.

### phase 2 : Write invalid msg
If an attacker enters invalid msg, the msg will be written to the file as an
error msg.

### phase 3 : EIP control
An attacker can open msg file from phase 2, which triggers BOF.

### phase 4: ROP
open, read, write, to read the flag.
