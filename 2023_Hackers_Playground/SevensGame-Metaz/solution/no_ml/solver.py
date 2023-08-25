from pwn import *
import sys
import time
import random
import tensorflow as tf
from tensorflow.keras import datasets, layers, models
import os
import numpy as np

tf.get_logger().setLevel('ERROR')
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
context.log_level = 'error'

STR_SETTING = "Select the settings menu you want to change, or input any other key to go back into the main menu: \n"
STR_MENU = "Select the menu: \n"

STR_DIAMOND = b'\x1b[33m \xe2\x96\x97\xe2\x96\x86\xe2\x96\x86\xe2\x96\x86\xe2\x96\x96 \x1b[37m'

BATCH_SIZE = 10

def int2digitlist(x, digit=8):
    return list(map(int, (("{0:0%db}"%digit).format(x))))

trial = 0

start = time.time()

while 1:
    trial += 1
    print("Trial #%d" % trial)
    #p = process(["./seven3", "2000", "100", "0"])
    p = remote("sevensgamemetaz76775d33.sstf.site", 7777)
    p.sendline("5\n5\n3\n9\n0\n0\n1\n")
    stripss = p.recvuntil(STR_MENU)
    strips = [[], [], []]
    p.recvuntil(STR_MENU)
    result = (p.recvuntil(STR_MENU))
    results = result.split(b"SCORE ")

    last = -1
    for i in range(1, 233):
        old = (results[i].split(b"+-------+-------+-------+")[1])
        old = old.replace(b"\033[A", b"")
        olds = old.split(b"\n")
        code1 = (olds[2].split(b"\x1b[46m|")[0][8:10])
        if code1 == b"34":
            if last != 0:
                strips[0].append(0)
                last = 0
        elif code1 == b"37":
            if last != 1:
                strips[0].append(1)
                last = 1
        elif code1 == b"31":
            if last != 2:
                strips[0].append(2)
                last = 2
        else:
            if last != 5:
                strips[0].append(5)
                last = 5

    strips[0] = strips[0][0:64]
    if len(strips[0]) < 64:
        print("Exit - Short strips[0]")
        p.close()
        continue

    last = -1
    for i in range(1, 433):
        old = (results[i].split(b"+-------+-------+-------+")[1])
        old = old.replace(b"\033[A", b"")
        olds = old.split(b"\n")
        code1 = (olds[2].split(b"\x1b[46m|")[1][2:4])
        if code1 == b"34":
            if last != 0:
                strips[1].append(0)
                last = 0
        elif code1 == b"37":
            if last != 1:
                strips[1].append(1)
                last = 1
        elif code1 == b"31":
            if last != 2:
                strips[1].append(2)
                last = 2
        else:
            if last != 5:
                strips[1].append(5)
                last = 5

    last = -1
    for i in range(1, 633):
        old = (results[i].split(b"+-------+-------+-------+")[1])
        old = old.replace(b"\033[A", b"")
        olds = old.split(b"\n")
        code1 = (olds[2].split(b"\x1b[46m|")[2][2:4])
        if code1 == b"34":
            if last != 0:
                strips[2].append(0)
                last = 0
        elif code1 == b"37":
            if last != 1:
                strips[2].append(1)
                last = 1
        elif code1 == b"31":
            if last != 2:
                strips[2].append(2)
                last = 2
        else:
            if last != 5:
                strips[2].append(5)
                last = 5

    strips[0].append(strips[0][0])
    strips[0] = strips[0][1:65]
    strips[0] = (list(reversed(strips[0])))
    strips[1].append(strips[1][0])
    strips[1] = strips[1][1:65]
    strips[1] = (list(reversed(strips[1])))
    strips[2].append(strips[2][0])
    strips[2] = strips[2][1:65]
    strips[2] = list(reversed(strips[2]))
    #print(strips)
    #print(stripss.decode())

    numbers = []
    for r1 in range(0, 32):
        if strips[0][r1 * 2] != 0:
            continue
        for r2 in range(0, 32):
            if strips[1][r2 * 2] != 0:
                continue
            for r3 in range(0, 32):
                if strips[2][r3 * 2] != 0:
                    continue
                numbers.append((r1 * 2) | ((r2 * 2) << 6) | ((r3 * 2) << 12))

    numbers = list(sorted(numbers, key=lambda x: abs(list(int2digitlist(x, 18)).count(1) - 10)))

    p.sendline("5\n2\n5\n0\n")
    (p.recvuntil(STR_MENU))

    for number in numbers[0:99]:
        p.sendline("5\n4\n%d\n0\n1\n1\n" % number)
        (p.recvuntil(STR_MENU))
        before = (p.recvuntil(STR_MENU).decode())
        after = (p.recvuntil(STR_MENU).decode())

        if "result_code : 3" in before:
            print("Confirm the result of BBB!")
        else:
            continue
        before = before.split("targets : ")[1].split(", symbol_count")[0]
        after = after.split("targets : ")[1].split(", symbol_count")[0]
        print(before, after)
        if before == after:
            print("Confirm the loop!")
            p.sendline("2\n10000\n")
            p.recvuntil(STR_MENU)
            p.sendline("5\n4\n%d\n7\n337\n0\n1\n1\n1\n" % number)
            p.recvuntil(STR_MENU)
            p.recvuntil(STR_MENU)
            p.recvuntil(STR_MENU)
            p.recvuntil(STR_MENU)
            p.sendline("3\n1\n0\n")
            print("##########BUY FLAG##########")
            print(p.recvuntil(STR_MENU).decode("utf8"))
            sys.exit()
        else:
            continue

    p.close()

    """
    print("what")
    break
    print(len(numbers))
    print(strips[0].count(0))
    print(strips[1].count(0))
    print(strips[2].count(0))

    print(bin(numbers[0]))
    print(bin(numbers[100]))
    print(bin(numbers[200]))
    print(bin(numbers[-1]))
    """
