from pickle import loads
import sys
from os import chroot, chdir, setresgid, setresuid
from seccomp import *

UID = 65535

f = SyscallFilter(defaction=KILL)
f.add_rule(ALLOW, "open")
f.add_rule(ALLOW, "read")
f.add_rule(ALLOW, "write", Arg(0, EQ, 1))
f.add_rule(ALLOW, "write", Arg(0, EQ, 2))
f.add_rule(ALLOW, "close")
f.add_rule(ALLOW, "ioctl")
f.add_rule(ALLOW, "stat")
f.add_rule(ALLOW, "fstat")
f.add_rule(ALLOW, "lseek")
f.add_rule(ALLOW, "kill")
f.add_rule(ALLOW, "brk")
f.add_rule(ALLOW, "rt_sigaction")
f.add_rule(ALLOW, "exit_group")

setresgid(UID, UID, UID)
setresuid(UID, UID, UID)

f.load()

C4n_y0u_r34d_7h15 = 'SCTF{3a5y_e4zy_p1ckl1ng}'

filtering = ['__dict__']

inp = ''
while True:
    one_byte = sys.stdin.read(1)
    if one_byte == '#':
        break
    inp += one_byte

if len(inp) > 60:
    print 'too long'
    exit()

if any(item in inp for item in filtering):
    print 'filtered'
    exit()

inp = loads(inp)
print 'Your inp is {}'.format(inp)

if not isinstance(inp, list) or sum(inp) != 10:
    print 'Your inp must be a list which sum of elements is 10'
    print 'Try again :P'
else:
    print 'Good Job!'
    print 'Now you are a python expert!'
