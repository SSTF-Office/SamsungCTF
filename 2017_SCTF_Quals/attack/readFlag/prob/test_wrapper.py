#!/usr/bin/python2
import os
os.system("stdbuf -i 0 -o 0 python test.py 2>&1")
