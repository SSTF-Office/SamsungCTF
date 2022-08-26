#!/usr/bin/env python3
from pwn import *
from queue import PriorityQueue
import sys

LOCAL = False
SERVER_DOMAIN = "127.0.0.1"
SERVER_PORT = 8098

class Challenge:
    goal = "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P"
    status = "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P"
    xpos = 0
    ypos = 0

    def calcScore(self):
        score = 0
        goal = self.goal.replace(",", "")
        status = self.status.replace(",", "")
        for index, ch in enumerate(goal):
            if ch != status[index]:
                score = score + 1
        return score

    def init(self):
        self.status = self.goal

    def __init__(self, status):
        self.setStatus(status)

    def setStatus(self, status):
        self.status = status
        arr = self.status.split(",")
        for index, ch in enumerate(arr):
            if ch == 'A':
                self.xpos = index // 4
                self.ypos = index % 4
                break

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

class Data:
    status = ""
    prev = ""
    dx = 0
    dy = 0
    dist = 0
    def __init__(self, status, prev, dx, dy, dist):
        self.status = status
        self.prev = prev
        self.dx = dx
        self.dy = dy
        self.dist = dist
    def __eq__(self, obj):
        return obj.status == self.status and obj.prev == self.prev
    def __lt__(self, obj):
        return self.status < obj.status

def solve2(stat):
    pass

def solve(initState):
    moves = [(0, +1), (0, -1), (+1, 0), (-1, 0)]
    visits = {}
    pq = PriorityQueue()
    chall = Challenge(initState)
    pq.put( (chall.calcScore(), Data(chall.status, None, None, None, 0) ) )
    while pq.qsize() > 0:
        score, data = pq.get()
        if visits.get(data.status) == None:
            visits[data.status] = (data.dx, data.dy)
        else:
            continue
        if data.status == "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P":
            break
        chall.setStatus(data.status)
        for dx, dy in moves:
            prev_status = chall.status
            assert prev_status == data.status
            chall.move(dx, dy)
            if visits.get(chall.status) == None:
                pq.put((chall.calcScore() + data.dist + 1, Data(chall.status, prev_status, dx, dy, data.dist + 1)))
            chall.move(-dx, -dy)
            assert prev_status == chall.status

    ptr = Challenge("A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P")
    answer = []
    while visits.get(ptr.status) != None:
        move = visits[ptr.status]
        dx, dy = move
        if dx == None:
            break
        answer.append(move)
        ptr.move(-dx, -dy)
    return answer[::-1]



if LOCAL:
    p = process('../attachment/app.py')
else:
    p = remote(SERVER_DOMAIN, SERVER_PORT)

for _ in range(100):
    p.recvuntil(":\n")
    initialStat = p.recvuntil(b">>>")[:-3].replace(b"\n", b"")
    arr = []
    print (initialStat)
    for c in initialStat:
        arr.append(chr(c))
    stat = ",".join(arr)
    answers = solve(stat)
    print (stat)
    print (answers)
    answers = map(lambda x : ",".join(map(str, x)), answers)
    for answer in answers:
        p.sendline(answer)
    resp = p.recvuntil(b"\n")
    if b"Solved" in resp:
        print ("Cool")
    else:
        print (resp)
        p.interactive()

print (p.recvuntil(b"}").decode().replace(">", ""))
p.close()
