from random import choice
import sys

def chg_format(n):
    if n == 0:
        return '    '
    elif n == 1:
        return 'o   '
    elif n == 2:
        return ' o  '
    elif n == 3:
        return '  o '
    elif n == 4:
        return '   o'

    print 'note error'
    exit(1)

cands = [0] * 28 + [1, 2, 3, 4]

def main():
    if len(sys.argv) != 2:
        print 'few arguments'
        exit(1)
    notes = [chg_format(choice(cands)) for i in xrange(int(sys.argv[1]))]

    for i, v in enumerate(notes):
        print '"{}",'.format(v),
        if i % 8 == 7:
            print
    return

if __name__ == '__main__':
    main()
