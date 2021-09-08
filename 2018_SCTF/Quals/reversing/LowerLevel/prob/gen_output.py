import string

def f(x):
    return string.uppercase.find(x)

segtable = [\
    [1,1,1,1,1,1,0],\
    [0,1,1,0,0,0,0],\
    [1,1,0,1,1,0,1],\
    [1,1,1,1,0,0,1],\
    [0,1,1,0,0,1,1],\
    [1,0,1,1,0,1,1],\
    [1,0,1,1,1,1,1],\
    [1,1,1,0,0,0,0],\
    [1,1,1,1,1,1,1],\
    [1,1,1,1,0,1,1]
]

ipt = "NUMBERDECODERINBREADBOARD"
for i in ipt:
    ipt = format(f(i), '#07b')[2:]
    num = int(ipt[:2] + ipt[3:],2)
    if ipt[2] == "1":
        print "0" + "".join(map(str, map(lambda x: 1-x, segtable[num])))
    else:
        print "0" + "".join(map(str, segtable[num]))
