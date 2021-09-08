# SDN-Scan

## Description

You are network admin of a secret intelligence agency.
There is a TCP service for the agents abroad to communicate.
Agents know the IP and port of this service,
but you don't want others to know about it. Even its existence.
(link) is a sample packet capture containing both scanner and benign connections..
Make sure port scanners do not find out about this TCP service.
And never block benign traffic.

## Comments

Detect and block a port scanner.

According to the traffic sample, the scanner simply connects to random TCP ports.

You can either:

- Detect RST-ACK from the server (connection refused), and block the client IP.

- Remember which IP connects to how many ports. If an IP tries to connect
  more than certain number of ports, block the IP.

