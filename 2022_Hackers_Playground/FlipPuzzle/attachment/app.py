#!/usr/bin/env python3

import random
import os
import signal
import sys

LIMIT_TIME = 50
NUM_STAGE = 100
SHUFFLE_NUM = 11

def bye():
    print ("Bye~")
    sys.exit()

signal.signal(signal.SIGALRM, bye)
signal.alarm(LIMIT_TIME)

class Challenge:
    goal = "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P"
    status = "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P"
    xpos = 0
    ypos = 0
    dist = 0

    def init(self):
        self.status = self.goal

    def shuffle(self, num):
        options = [(0, +1), (0, -1), (+1, 0), (-1, 0)]
        for _ in range(num):
            dx, dy = random.choice(options)
            self.move(dx, dy)

    def move(self, dx, dy):
        assert abs(dx + dy) == 1
        assert dx == 0 or dy == 0
        arr = self.status.split(",")
        p1 = self.xpos * 4 + self.ypos
        xxpos = (self.xpos + dx + 4) % 4
        yypos = (self.ypos + dy + 4) % 4
        p2 = xxpos * 4 + yypos

        arr[p1], arr[p2] = arr[p2], arr[p1]
        self.xpos = xxpos
        self.ypos = yypos
        self.status = ",".join(arr)

    def ok(self):
        return self.goal == self.status

    def dump(self):
        arr = self.status.split(",")

        for i in range(0, 4):
            print ("".join(arr[i*4:i*4+4]))

for _ in range(NUM_STAGE):
    chall = Challenge()
    chall.shuffle(SHUFFLE_NUM)
    cnt = 0
    print("Current Status :")
    chall.dump()
    while chall.ok() == False:
        try:
            dx, dy = map(int, input(">>>").split(","))
            chall.move(dx, dy)
            cnt = cnt + 1
            if cnt > SHUFFLE_NUM:
                bye()
        except:
            bye()
    print ("Solved!")

print("SCTF{fake-flag}")