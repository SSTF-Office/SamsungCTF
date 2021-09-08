import pyqrcode
from random import random
from base64 import b64encode as b64e
from time import sleep
import signal
import sys

DEFAULT_LEN = 60
VERSION = 7
TIME_LIMIT = 40
STAGE = 100
TOO_SLOW = '''
  _______                _                   _____
 |__   __|              | |                _|  __ \\
    | | ___   ___    ___| | _____      __ (_) |__) |
    | |/ _ \\ / _ \\  / __| |/ _ \\ \\ /\\ / /   |  ___/
    | | (_) | (_) | \\__ \\ | (_) \\ V  V /   _| |
    |_|\\___/ \\___/  |___/_|\\___/ \\_/\\_/   (_)_|
'''

INTRO_MSG = '''Welcome to 3-lights-out hell :P
100 stages are here.
Give me data encoded by qrcode.
You can find qrcode when you make board all red.

Good Luck'''

def gen_rand_str(n):
    with open('/dev/urandom', 'r') as f:
        data = b64e(f.read(n))[:n]

    return data

def gen_qrcode(n=DEFAULT_LEN):
    data = gen_rand_str(n)
    qrdata = pyqrcode.create(data, version=VERSION)

    return qrdata.code, data

def gen_click_map():
    map, data = gen_qrcode()

    for i in xrange(len(map)):
        for j in xrange(len(map[0])):
            map[i][j] *= 2 if random() > 0.5 else 1

    return map, data

class TLight:
    def __init__(self, map):
        self.table = {0: 'R', 1: 'G', 2: 'B'}
        self.map = map
        self.h = len(self.map)
        self.w = len(self.map[0])
        self.board = [[0 for j in xrange(self.w)] for i in xrange(self.h)]
        return

    def click(self, x, y):
        dist = [[-1, 0], [-1, 0],
                [0, 1], [0, 1],
                [0, 0],
                [0, -1], [0, -1],
                [1, 0], [1, 0]]

        for dx, dy in dist:
            if 0 <= dx+x < self.h and 0 <= dy+y < self.w:
                self.board[dx+x][dy+y] += 1
                self.board[dx+x][dy+y] %= 3

        return

    def mk_board(self):
        for i in xrange(self.h):
            for j in xrange(self.w):
                if self.map[i][j] == 1:
                    self.click(i, j)
                elif self.map[i][j] == 2:
                    self.click(i, j)
                    self.click(i, j)

        return

    def pprint(self):
        for line in self.board:
            print ''.join(self.table[i] for i in line)
        return

def handler(signum, frame):
    print TOO_SLOW
    exit(0)
    return

def main():
    print INTRO_MSG
    print

    sleep(3)

    signal.signal(signal.SIGALRM, handler)
    signal.alarm(TIME_LIMIT)

    for i in xrange(STAGE):
        print 'STAGE ({}/{})'.format(i+i, STAGE)
        click_map, answer = gen_click_map()
        t = TLight(click_map)
        t.mk_board()

        t.pprint()

        get_answer = raw_input('answer: ')
        if answer != get_answer:
            print 'try again :P'
            return

    print 'yay! Now you will get a flag'
    with open('flag', 'r') as f:
        print f.read()

    return

if __name__ == '__main__':
    main()
