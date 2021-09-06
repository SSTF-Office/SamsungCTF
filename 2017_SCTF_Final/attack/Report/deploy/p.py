import struct
from pwn import *
import re

PatchAddr = []
PatchList = []
AddtionalData = []
BaseAddr = 0x400000
TotalData = ""
FileName = "./a.out"
OutName = "./Report"

def FindVtableRange(p):
    VtableMin = 0xffffffff
    VtableMax = 0xffffffff
    for sym in p.symbols:
        if bool(re.search("_ZTV\d", sym)) and VtableMin > p.symbols[sym]:
            VtableMin = p.symbols[sym]
        if bool(re.search("_ZTI\d", sym)) and VtableMax > p.symbols[sym]:
            VtableMax = p.symbols[sym]
    return [VtableMin, VtableMax]


def ForwardFind(Foffset):
    Data = ""
    Foffset += 5
    while True:
        if TotalData[Foffset] == "\x90":
            break
        if TotalData[Foffset] != "\x90":
            Data += TotalData[Foffset]
        Foffset += 1
    return Data 



p = ELF(FileName)


VtableErrorAddr = p.symbols['_Z11VtableErrorv']
Bound = FindVtableRange(p)

CheckPattern = "4889d7ffd0".decode('hex') #mov     rdi, rdx,   call rax

f = open(FileName, "rb")
TotalData = f.read()
f.close()

ind = -1
while True:
    ind = TotalData.find(CheckPattern, ind+1)
    if ind < 0:
        break
    PatchAddr.append(ind)

print map(lambda x:hex(x+BaseAddr), PatchAddr)

print map(hex, Bound)

for pa in PatchAddr:
    AddtionalData.append(ForwardFind(pa))

print AddtionalData

for pa in PatchAddr:
    PatchList.append(asm("""mov rdi, rdx
push rax
mov rax, Qword PTR[rdi]
cmp rax, %d
jb %d
cmp rax, %d
ja %d
pop rax
call rax""" % (Bound[0], VtableErrorAddr, Bound[1], VtableErrorAddr), arch="amd64", vma=pa + BaseAddr))


TotalData = bytearray(TotalData)

for i in xrange(len(PatchAddr)):
    pa = PatchAddr[i]
    pd = PatchList[i]
    ad = AddtionalData[i]
    AddtionalLen = len(pd) + len(ad)
    TotalData[pa:pa+AddtionalLen] = pd+ad

f = open(OutName, "wb")
f.write(TotalData)
f.close()
