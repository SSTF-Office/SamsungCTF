#!/usr/bin/env python

import sys
import socket
import random
import struct

up = lambda x : struct.unpack('<L', x)[0]

HOST = ''
PORT = 443

def parse_packet(data):
  return up(data[:4]), data[4:]

def main():
  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  sock.bind((HOST, PORT))
  sock.listen(10)

  file_idx = 0
  while True:
    client, client_info = sock.accept()
    print("[*] client {} connected!".format(client_info))

    f = open(str(file_idx).rjust(2, '0'), 'wb')

    while True:
      read_data = client.recv(1024)
      data_size, data = parse_packet(read_data[15:43])

      if data_size == 0:
        break

      if random.randint(0, 100) < 5:
        client.send(random.choice('0'))
      else:
        client.send(random.choice('1'))
        f.write(data)

    file_idx += 1
    f.close()
    client.close()
    print("[*] client closed")

if __name__ == '__main__':
  main()
