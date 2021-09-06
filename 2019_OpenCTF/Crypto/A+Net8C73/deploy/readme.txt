[Info]
 Title:A+Net8C73
 Description: Someone tried to connect to the WiFi AP(SSID: A+Net8C73), but the connection failed because the password did not match.
However, you may be able to find the password for WiFi AP connection.
The format of the password is SCTF {number}, where number is a five-digit decimal number.
p.s. Dictionary Attack can be a powerful attack when you are in offline without interactions with the server.
So we can limit the number of flag submissions up to 10 times.
 Attach: A+Net8C73.pcap

[Deploy]
 Not necessary

[Solving Strategy]
 1. Get PMKID and MAC addresses of both sides from packets in the file.
 2. For each guess for the password, derive PMKID using SSID and MAC addresses and then check the equality of them.

