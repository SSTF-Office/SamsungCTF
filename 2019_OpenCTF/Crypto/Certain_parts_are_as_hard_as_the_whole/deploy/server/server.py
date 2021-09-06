import socketserver

from public import public
from secret import secret

welcome = b'''\
========================================
RSA LSB Oracle
========================================
If you provide a ciphertext, I will give
you the LSB of corresponding plaintext.\n'''

counter = 0
(n, e, y) = public
(d, p, q, dp, dq, p1q0, p0q1) = secret


def server_worker(y):
    global counter, p, q, dp, dq, p1q0, p0q1
    counter += 1
    y1 = y%p; y2 = y%q
    x1 = pow(y1, dp, p); x2 = pow(y2, dq, q)
    x = (x1*p1q0 + x2*p0q1)%n
    #assert y == pow(x, e, n)
    return x%2

def handle(self):
    self.write(welcome)
    while(True):
        message = "{}-th Ciphertext: ".format(counter)
        ct = self.query(message.encode())
        bit = server_worker(int(ct, 16))
        resp = "LSB: {}".format(bit)
        self.write(resp.encode())

class RequestHandler(socketserver.BaseRequestHandler):

	handle = handle

	def read(self, until=b'\n'):
		out = b''
		while not out.endswith(until):
			out += self.request.recv(1)
		return out[:-len(until)]

	def query(self, string=b''):
		self.write(string, newline=False)
		return self.read().decode()

	def write(self, string, newline=True):
		self.request.sendall(string)
		if newline:
			self.request.sendall(b'\n')

class Server(socketserver.ForkingTCPServer):

	allow_reuse_address = True

	def handle_error(self, request, client_address):
		pass

port = 4003
server = Server(('0.0.0.0', port), RequestHandler)
server.serve_forever()