#!/usr/bin/python
from myDSA import import_key
import signal
import sys


def wline(msg=''):
    sys.stdout.write(msg + '\n')
    sys.stdout.flush()


def rline(msg=None):
    if msg is not None:
        wline(msg)

    line = ''
    while '\n' not in line:
        line += sys.stdin.readline()
    return line.rstrip()


class tinyAuthServer:
    def __init__(self, signer, secret_file):
        self.signer = signer
        self.secret_file = secret_file

    def register(self, username):
        return self.signer.sign(username)

    def login(self, token, username):
        return self.signer.verify(username, token)

    def handle(self):
        try:
            while True:
                choice = rline('1) register 2) login 3) exit')
                choice = int(choice)

                if choice == 1:
                    hx = rline('Give me your name as hex encoded string.')
                    username = bytes.fromhex(hx)

                    if b'admin' in username:
                        wline('admin is not allowed')
                        return

                    token = self.register(username)
                    wline('Your token is here. Plz keep it secret.')
                    wline('%d %d' % token)

                elif choice == 2:
                    hx = rline('Give me your name as hex encoded string.')
                    username = bytes.fromhex(hx)

                    token = rline('Give me your token as %d %d format.')
                    token = map(int, token.split(' ', 1))

                    if self.login(token, username):
                        wline('login success')
                    else:
                        wline('login failure')
                        return

                    if username == b'admin':
                        with open(self.secret_file, 'r') as f:
                            wline(f.read())

                else:
                    return

        except Exception:
            exit()


if __name__ == '__main__':
    if len(sys.argv) != 3:
        wline('Usage: %s <path_to_private_key> <path_to_secret>')
        exit(-1)

    with open('./salt.bin', 'rb') as f:
        salt = f.read(12)
        assert len(salt) == 12

    with open(sys.argv[1], 'r') as f:
        signer = import_key(f.read(), salt)

    server = tinyAuthServer(signer, sys.argv[2])
    signal.alarm(60)
    server.handle()
