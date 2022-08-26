from pwn import *
import sys

context.log_level = 'error'

# Phase #1 : Gather internal game data
# Assume that the attacker realizes that the bonus game has 5 combinations.
while 1:
    allcases = {}
    #p = process("./seven1")
    p = remote("sevensgamelow.sstf.site", 7777)
    p.sendline("5")
    p.sendline("2")
    p.sendline("0")
    p.sendline("2")
    p.sendline("500")
    p.recv()

    while 1:
        p.sendline("1")
        msg = b""
        while 1:
            msg += p.recvline()
            if b"Choose one." in msg:
                content = msg.split(b"Choose one.")[-2].split(b"Win the Bonus Game!")[1]
                p.sendline("1")
                msg = b""
                while 1:
                    msg += p.recvline()
                    if b"Welcome to Seven's Game for Low Roller!" in msg:
                        content1 = msg.split(b"Actual score is multiplied by")[-2].split(b"Bonus Game Result")[1].replace(b"\033", b"").replace(b"[34m", b"").replace(b"[37m", b"").replace(b"[31m", b"").replace(b"[33m",b"").decode("latin-1")
                        cases = content1.split("|")
                        cases = list(map(lambda x: int(x.strip()), cases[1:3]+cases[4:6]))
                        allcases[content] = {"min":cases.index(min(cases)) + 1, "max":cases.index(max(cases)) + 1}
                        break
                break
            elif b"Welcome to Seven's Game for Low Roller!" in msg:
                # go to main
                break
        if b"You have played too much!" in msg:
            break
        if b"Your score is too low to play." in msg:
            break

    if (len(allcases) == 5):
        break
print("Phase #1 Done")

#print(allcases)
#print(len(allcases))

# Phase #2 : 5000 -> 50000
while 1:
    #p = process("./seven1")
    p = remote("sevensgamelow.sstf.site", 7777)
    p.sendline("5")
    p.sendline("2")
    p.sendline("0")
    p.sendline("2")
    p.sendline("5000")

    spin_count = 0
    score = 0

    msg = b""
    while 1:
        msg += p.recv()
        #print(len(msg.split(b"\n")))
        if (len(msg.split(b"\n")) == 43):
            break

    while 1:
        p.sendline("1")
        spin_count += 1
        msg = b""
        while 1:
            msg += p.recvline()
            if b"SCORE  " in msg:
                score = int(msg.split(b"SCORE")[1].split(b"\n")[0].strip().decode("latin-1").replace(",",""))
            if b"Choose one." in msg:
                content = msg.split(b"Choose one.")[-2].split(b"Win the Bonus Game!")[1]
                p.sendline(str(allcases[content]['max']))
                msg = b""
                while 1:
                    msg += p.recvline()
                    if b"Welcome to Seven's Game for Low Roller!" in msg:
                        break
                break
            elif b"Welcome to Seven's Game for Low Roller!" in msg:
                # go to main
                break
        if score > 50000:
            break
        if b"You have played too much!" in msg:
            #print("Play time limit")
            break
        if b"Your score is too low to play." in msg:
            #print("No money")
            break
    if score > 50000:
        print("Phase #2 Done")
        break

# Phase #3 : 50000 -> 5M
p.sendline("2")
p.sendline("50000")

msg = b""
while 1:
    msg += p.recv()
    if (len(msg.split(b"\n")) == 21):
        break
while 1:
    if (spin_count == 599):
        p.sendline("3")
        p.sendline("1")
        msg = b""
        while 1:
            msg+=p.recv()
            if b"Not enough" in msg:
                print("Try again!")
                sys.exit(-1)
            elif b"SCTF" in msg:
                print("SCTF"+msg.split(b"SCTF")[1].split(b"\n")[0].decode("latin-1"))
                sys.exit(0)
    p.sendline("1")
    spin_count += 1
    msg = b""
    while 1:
        #print(msg)
        msg += p.recvline()
        if b"Choose one." in msg:
            content = msg.split(b"Choose one.")[-2].split(b"Win the Bonus Game!")[1]
            p.sendline(str(allcases[content]['min']))
            msg = b""
            while 1:
                msg += p.recvline()
                if b"Welcome to Seven's Game for Low Roller!" in msg:
                    break
            break
        elif b"Welcome to Seven's Game for Low Roller!" in msg:
            # go to main
            break
    if b"You have played too much!" in msg:
        print("Try again! - Play time limit")
        sys.exit(-1)
    if b"Your score is too low to play." in msg:
        print("Try again! - No money")
        sys.exit(-1)
