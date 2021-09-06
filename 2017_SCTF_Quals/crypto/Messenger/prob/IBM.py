from SocketServer import ThreadingTCPServer, StreamRequestHandler
from Crypto.Util import number
from Crypto.Hash import SHA256, MD5
import os
import random
import sys


class IdBasedMessenger():
    def __init__(self, pk=None, sk=None):
        self.MsgDir = 'messages/'
        if pk is None:
            from Crypto.PublicKey.RSA import generate
            rsa = generate(2048)
            pk = rsa.n
        self.pk = pk

        if sk is None:
            sk = 2 * number.getRandomNBitInteger(255) + 1
        self.sk = sk

    def hid(self, name):
        return number.bytes_to_long(SHA256.new(name).digest())

    def register(self, name):
        if not name.isalnum():
            raise TypeError

        hid = self.hid(name)
        return pow(self.sk, hid, self.pk)

    def login(self, name, rnd, sign):
        if not name.isalnum():
            raise TypeError

        if not os.path.isfile(self.MsgDir + name):
            open(self.MsgDir + name, 'a').close()

        hid = self.hid(name)
        return pow(self.sk, hid * rnd, self.pk) == sign

    def readMsg(self, name):
        if not name.isalnum():
            raise TypeError
        with open(self.MsgDir + name) as f:
            return f.read()

    def writeMsg(self, src, dst, msg):
        if not src.isalnum() or not dst.isalnum():
            raise TypeError

        with open(self.MsgDir + dst, 'a') as f:
            f.write('Msg From {} : {}\n'.format(src, msg))
        return


class IBM_Handler(StreamRequestHandler):
    def wline(self, msg):
        self.wfile.write(msg + '\n')
        self.wfile.flush()

    def rline(self, msg=None):
        if msg is not None:
            self.wline(msg)
        return self.rfile.readline().strip()

    def ProofOfWork(self):
        chal = ''.join(chr(random.randint(0, 0xff)) for _ in range(6))
        self.wline('Challenge : {}'.format(chal.encode('hex')))
        try:
            to_check = self.rline().decode('hex')
        except TypeError:
            return False

        if to_check[:6] != chal:
            return False

        return MD5.new(to_check).hexdigest().endswith('fffff')

    def handle_register(self):
        name = self.rline('id?')
        if name == 'admin':
            self.wline('admin is not allowed')
            return
        try:
            sk = MESSENGER.register(name)
            self.wline('Server public key : {:x}'.format(MESSENGER.pk))
            self.wline('Your secret key : {:x}'.format(sk))
        except TypeError:
            self.wline('Nope')
        return

    def handle_login(self, option):
        name = self.rline('You need to login first. What is your id?')
        rnd = 2 * number.getRandomNBitInteger(255) + 1
        self.wline('Pls sign on it : {:x}'.format(rnd))
        try:
            sign = int(self.rline(), 16)
            if MESSENGER.login(name, rnd, sign):
                self.wline('Welcome {}'.format(name))
            else:
                self.wline('Login failed')
                return
        except TypeError:
            self.wline('Nope')
            return

        if option == 'read':
            self.handle_read(name)

        elif option == 'write':
            self.handle_write(name)

        return

    def handle_read(self, name):
        self.wline(MESSENGER.readMsg(name))
        return

    def handle_write(self, name):
        dst = self.rline('To whom do you want to send?')
        msg = self.rline('Msg?')
        try:
            MESSENGER.writeMsg(name, dst, msg)
            self.wline('Success')
        except TypeError:
            self.wline('Nope')
        return

    def handle(self):
        if not self.ProofOfWork():
            self.wline('Nope')
            return

        self.wline('Welcome to new Identity Based Messenger - IBM!')
        self.wline('(1) register\n(2) read message\n(3) send message')
        try:
            choice = int(self.rline())
        except TypeError:
            self.wline('Nope')
            return

        if choice == 1:
            self.handle_register()

        elif choice == 2:
            self.handle_login('read')

        elif choice == 3:
            self.handle_login('write')

        else:
            self.wline('Nope')
        return


if __name__ == '__main__':
    ADDR, PORT = '0.0.0.0', 31337
    FLAG = sys.argv[1] if len(sys.argv) > 1 else 'SAMPLE_FLAG'
    try:
        os.mkdir('messages')
    except OSError:
        pass
    finally:
        with open('messages/admin', 'w') as f:
            f.write(FLAG)

    MESSENGER = IdBasedMessenger()
    ThreadingTCPServer.allow_reuse_address = True
    server = ThreadingTCPServer((ADDR, PORT), IBM_Handler)
    server.serve_forever()
