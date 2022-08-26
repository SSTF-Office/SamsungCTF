import os
import time

SOLVERS = 10
count = [0] * SOLVERS
phase1 = [0] * SOLVERS
phase2 = [0] * SOLVERS

for i in range(SOLVERS):
    os.system("python3 -u solver.py > result%d 2>/dev/null &" % i)
start = time.time()

while 1:
    for i in range(SOLVERS):
        msg = os.popen('tail result%d | grep "Phase 1 : "' % i).read()
        if len(msg) == 0:
            continue
        p1, t = msg.split(": ")[1].split(" / ")
        phase1[i] = int(p1)
        count[i] = int(t.split(" ")[0])
        print(sum(phase2), sum(phase1), sum(count), sum(phase1) / sum(count) * 100, (time.time() - start) / sum(count))
        time.sleep(1)

        msg = os.popen('tail result%d | grep "Phase 2 : "' % i).read()
        if len(msg) == 0:
            continue
        p2 = msg.split(": ")[1].split(" ")[0]
        phase2[i] = int(p2)
        if (int(p2) > 0):
            os.system("tail result%d | grep SCTF" % i)

