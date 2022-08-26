#!/usr/bin/env python3

from os.path import join, dirname
from pwn import *

ROOT = dirname(__file__)
PORT = open(join(ROOT, "../PORT")).read()
FLAG = open(join(ROOT, "flag"), "rb").read().strip()

p = process([join(ROOT, "../src/exploit.py")],
            env={"PORT": PORT, "REMOTE": "1"})
out = p.recvall()

if FLAG in out:
    print("[!] OK. (found: %s)" % FLAG)
else:
    print("[!] FAILED.\n%s" % out)
