import socket
import random
from secret import flag

message = "Diffie Hellman is one of the earliest practical examples of public key exchange. It is a very simple and good example for understanding discrete logarithm problem. Now the flag is %s."%flag

#make connection
host = '0.0.0.0'
port = 12345

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, port))
s.listen(1)
conn, addr = s.accept()

#receive public parameters(p, g)
msg = conn.recv(1024)
exec(msg)

#generate server private and server public
privS = random.randrange(1, p)
pubS = pow(g, privS, p)

#send server public to client
conn.sendall("pubS=%d"%pubS)

#receive client public
msg = conn.recv(1024)
exec(msg)

#calculate shared secret
sharedsecret = pow(pubC, privS, p)

#send encrypted message
msg = sharedsecret ^ int(message.encode("hex"), 16)
conn.sendall("sharedsecret_XORed_message=%d"%msg)

s.close()
