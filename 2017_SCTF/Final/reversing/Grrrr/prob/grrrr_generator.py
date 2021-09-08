import os
import sys

key = "Grumpy makes me suck!"

template = '''import os
import sys

key = "Grumpy makes me suck!"
if __name__ == "__main__":
        if len(sys.argv) != 2:
                print 'Usage grrrr.py [flag]'
        else:
                flag = sys.argv[1]
'''

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'Usage grrrr_generator.py [flag]'
    else:
        s = '\t' * 2 + 'if len(flag) == %d:\n' % len(sys.argv[1])
        for i in range(0, len(sys.argv[1])):
            s += '\t' * (3 + i) + 'if ord(flag[%d]) ^ %d == ord(key[%d]):\n' % (
                i, ord(sys.argv[1][i]) ^ ord(key[i % len(key)]), i % len(key))
        code = template + s
        code += '\t' * (3 + len(sys.argv[1])) + \
            'print "Congratulations!"\n'
        print code
