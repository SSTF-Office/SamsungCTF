[Info]
Title:Double Header
Description:What does DH means in Cryptography?<br><br>Can you see the secret message from the server?
Attach:Double_Header.zip

[Deploy]
 Not necessary

[Solving Strategy]
 1. extract pubS and sharedsecret_XORed_message from pcap file
 2. extract privC and p from client_params.dat file
 3. calculate shared secret according to Diffie-Hellman algorithm
 4. get flag by XORing shared secret and sharedsecret_XORed_message
