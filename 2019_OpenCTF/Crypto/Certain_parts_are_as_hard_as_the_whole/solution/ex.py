from Time import *
from Base import *
from Factorization import *
from Prime import *

SECRET = None
PUBLIC = None
COUNTER = 0

def GetInstance(l, flag=None):
    p1 = RandomPrime(l//2); p2 = RandomPrime(l//2)
    n = p1*p2
    q1 = p2*InverseMod(p2, p1)
    q2 = p1*InverseMod(p1, p2)
    m = (p1-1)*(p2-1)
    e = 0x10001
    d = InverseMod(e, m)
    l = l//8
    if flag == None:
        r = RandomInteger(0, n)
    else:
        r = (int.from_bytes(flag[:l].encode("utf-8"), "big"))%n
    y = pow(r, e, n)
    global SECRET, PUBLIC
    SECRET = [p1, p2, q1, q2, d, r]
    PUBLIC = [n, e, l, y]
    return

def LSBOracle(y):
    global SECRET, PUBLIC, COUNTER
    COUNTER += 1
    #x = pow(y, SECRET[2], PUBLIC[0])
    d1 = SECRET[4]%(SECRET[0]-1); d2 = SECRET[4]%(SECRET[1]-1)
    y1 = y%SECRET[0]; y2 = y%SECRET[1]
    x1 = pow(y1, d1, SECRET[0])
    x2 = pow(y2, d2, SECRET[1])
    x = (x1*SECRET[2] + x2*SECRET[3])%PUBLIC[0]
    return x%2


def ChosenCiphertext(n, e, y, r=None):
    if r == None:
        r = RandomInteger(0, n)
    else:
        r = r%n
    z = (pow(r, e, n)*y)%n
    return (r, z)

def DivideByTwo(a, y):
    #assert DecryptionOracle_LSB(u) == 0
    c = InverseMod(2, n)
    z = (y*pow(c, e, n))%n
    b = (a*c)%n if a%2 == 1 else a//2
    return (b, z)

def Attack(n, e, y):
    (a, u) = ChosenCiphertext(n, e, y)
    (b, v) = ChosenCiphertext(n, e, y)
    while u != 1 and v!= 1:
        if u == v or u == 0 or v == 0:
            print("Attack Failed!: the GCD is not equal to 1")
            return None
        lsb_u = LSBOracle(u)
        lsb_v = LSBOracle(v)
        while lsb_u*lsb_v == 0:
            if lsb_u == 0:
                (a, u) = DivideByTwo(a, u)
                lsb_u = LSBOracle(u)
            if lsb_v == 0:
                (b, v) = DivideByTwo(b, v)
                lsb_v = LSBOracle(v)
        (c, u) = ChosenCiphertext(n, e, y, a+b)
        if LSBOracle(u) != 0:
            print("Attack Failed: the sum is greater than modulus")
            return None
        (c, u) = DivideByTwo(c, u) 
        (d, v) = ChosenCiphertext(n, e, y, a-b)
        if LSBOracle(v) == 1:
            (d, v) = ChosenCiphertext(n, e, y, b-a)
        (d, v) = DivideByTwo(d, v)
        a = c; b = d
    return InverseMod(a, n) if u == 1 else InverseMod(b, n)
