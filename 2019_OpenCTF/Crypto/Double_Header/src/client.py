import subprocess
import socket
import random
'''
#generate DH parameters
keydata = subprocess.check_output("openssl dhparam -text -5 1024".split()).replace("\n", "").replace(" ", "")
keydata = keydata.split("prime:", 1)[1].split("-----", 1)[0][:-5]
p, g = keydata.split("generator:")
p = int(p.replace(":", ""), 16)
g = int(g)
'''
p=147948812730787892816553596449014257692735065898550127342067414978465789405923110624596658783997470719949020630001626003447695888000068621631517386034355740562123935174377804389506481916090552750177972374689102277473869147313524156902604114898465008298684803817663939694842359837470176415046500784487533851363
g=5

#make connection
host = '192.168.154.1'
port = 12345

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

#send public parameters
s.sendall("p,g=%d,%d"%(p, g))

#receive server public
msg = s.recv(1024)
exec(msg)

#generate client private and client public
privC = random.randrange(1, p)
pubC = pow(g, privC, p)

#write client params
with open("client_params.dat", "w") as f:
	f.write("p=%d\n"%p)
	f.write("g=%d\n"%g)
	f.write("privC=%d\n"%privC)
	f.write("pubC=%d\n"%pubC)

#send client public to server
s.sendall("pubC=%d"%pubC)

#calculate shared secret
sharedsecret = pow(pubS, privC, p)

#receive encrypted message
msg = s.recv(1024)
exec(msg)

message = sharedsecret_XORed_message ^ sharedsecret
print "message", hex(message)[2:-1].decode("hex")

s.close()
