[Info]
 Title: Certain parts are as hard as the whole
 Description: Each bit of RSA are said to be secure.
In other words, it is difficult to get a bit of plaintext from RSA ciphertext.
Repeating the same thing, you can find out the whole plaintext if you can get a bit of plaintext from the RSA ciphertext.
Especially, if you can figure out the LSB of the plaintext, you can find out the whole plaintext more easily.
server is running at: [serverIP] [port]

[Deploy]
 python3 server.py

[Solving Strategy]
 0. Google "RSA LSB Oracle".
 1. Get public parameters and a ciphertext from the file "public.py".
 2. Using the server as an RSA LSB oracle, recover the corresponding plaintext to the challenge ciphertext.

