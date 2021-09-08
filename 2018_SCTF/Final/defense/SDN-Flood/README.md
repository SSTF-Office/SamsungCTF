# SDN-Flood

## Description

You are network admin of a cloud service.
There is currently one server running, so there's not much work to do.
Unfortunately DDoS traffic has increased since last week.
You opt to write an SDN app that blocks DDoS packets.
(link) is a sample packet capture containing both attacker and benign traffic.
Make sure DDoS packets never reach the server.
And never block benign traffic.

## Comment

Defend a syn flooding attack as described in the sample packet.

An attacker sends many IP-spoofed SYN packets.

There are several ways to filter out the attack traffic.

To list a few:

- Write a TCP proxy as in <http://www.sdn-anti-spoofing.net/> (Intended solution).

- Inspect lengths of packets.

- Inspect TCP option fields.
