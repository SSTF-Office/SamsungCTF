import random

def chk1(x):
    is_zero = True
    for i in x:
        if is_zero:
            if i == '0':
                continue
            else:
                is_zero = False
        else:
            if i == '1':
                continue
            else:
                return False
    return True

def chk2(x):
    return all(map(lambda y: y == '1', x)) and (7 * len(x)) % 13 == 1

def chk3(x):
    is_zero = True
    v = 0
    for i in x:
        if is_zero:
            if i == '0':
                v += 1
                continue
            else:
                v -= 1
                is_zero = False
        else:
            if i == '1':
                v -= 1
                continue
            else:
                return False
    return v == 0

def chk4(n):
    if n==2 or n==3: return True
    if n%2==0 or n<2: return False
    for i in range(3,int(n**0.5)+1,2):   # only odd numbers
        if n%i==0:
            return False
    return True


def genTC1():
    TC1NUM = 777
    tcs = []
    for _ in range(TC1NUM):
        m, n = random.randint(0, 1000), random.randint(0, 1000)
        tc = '0' * m + '1' * n
        r = random.random()
        if r < 0.4:
            tc = list(tc)
            random.shuffle(tc)
            tc = ''.join(tc)
        elif r < 0.6:
            tc = '0' * m + '1' * n + '0' * random.randint(0, 1000)
        tcs.append(tc)
    random.shuffle(tcs)
    return list(zip(map(chk1, tcs), tcs))

def genTC2():
    TC2NUM = 666
    tcs = []
    for _ in range(TC2NUM):
        tc = '1' * random.randint(0, 1000)
        r = random.random()
        if r < 0.5:
            tc += '0' * random.randint(0, 1000)
            r = random.random()
            if r < 0.2:
                tc = list(tc)
                random.shuffle(tc)
                tc = ''.join(tc)

        tcs.append(tc)
    random.shuffle(tcs)
    return list(zip(map(chk2, tcs), tcs))

def genTC3():
    TC3NUM = 555
    tcs = []
    for _ in range(TC3NUM):
        tc = '0' * random.randint(0, 100)
        r = random.random()
        if r < 0.4:
            tc += '1' * len(tc)
        elif r < 0.6:
            tc += '1' * random.randint(0, 100)
            tc = list(tc)
            random.shuffle(tc)
            tc = ''.join(tc)
        else:
            tc += '1' * random.randint(0, 100)
        tcs.append(tc)
    random.shuffle(tcs)
    return list(zip(map(chk3, tcs), tcs))

def genTC4():
    TC4NUM = 100
    tcs = []
    for i in range(TC4NUM):
        tc = '0' * i
        tcs.append(tc)
    random.shuffle(tcs)
    return list(zip(map(chk4, tcs), tcs))

genTC = [genTC1, genTC2, genTC3, genTC4]
