import signal
import random


class LCG:
    def __init__(self, s, k):
        self.init_state = s

        k1, k2, k3 = k
        self.x = (k1 - 0xdeadbeef) % k3
        self.y = (k1 * 0xdeadbeef) % k3
        self.z = k2
        self.m = k3

    def __iter__(self):
        self.index = 0
        self.size = 32
        self.s0, self.s1 = self.init_state
        return self

    def __next__(self):
        if self.index >= self.size:
            raise StopIteration

        self.index += 1
        s0, s1 = self.s0, self.s1
        self.s0, self.s1 = s1, (self.x * s1 + self.y * s0 + self.z) % self.m
        return self.s1


if __name__ == '__main__':
    signal.alarm(60)

    s0, s1, k1, k2, k3 = [random.getrandbits(64) for i in range(5)]

    s = (s0, s1)
    k = (k1, k2, k3)

    cnt = 16
    for i, v in enumerate(LCG(s, k)):
        guess = int(input())
        if guess == v:
            cnt += 1
        else:
            cnt -= 1

        if i <= 16:
            print(v)

    if cnt >= 16:
        with open('flag.txt', 'r') as f:
            print(f.read())
