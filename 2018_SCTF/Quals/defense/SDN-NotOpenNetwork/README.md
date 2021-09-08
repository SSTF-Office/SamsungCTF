# SDN-Not Open Network
## Description

http://[SDN site]/Not_Open_Network

## How to run

Provided by SDN Site.

## Comments

Tutorial for building onos app.

Block packets by setup firewall.

This should be qualify round challenge.

## Flag

`SCTF{The_B4sic_0f_SDN_4pp}`

## How to solve

The given skeleton code (devenv) shows how to setup a PacketProcessor
that intercepts packets and selectively forward the packets.

Then the problem boils down to writing a correct filtering condition
that determines if a packet should be blocked or not.

The filtering logic is as simple as the problem description.
(1) Block any packet contains "police" (2) block any packet
that heads to 10.0.0.0/16 and ports other than 80.

You can use whatever method to implement above logic.
One way is to use `Ip4Prefix.contains` and
`String.toLowerCase().contains`.
