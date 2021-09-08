#!/usr/bin/env python
import re
import binascii
import os
import time
import sys
import tempfile
import subprocess


def msg(s):
    print s
    sys.stdout.flush()


def run(name):
    msg('running..')
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    cmd = ['./Speculator', '--add-treasure', 'kernel.bin', name]
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    if 0:
        msg(out)
        msg('--------------------')
        msg(err)
        msg('====================')

def get(f):
    msg('Size of the file in decimal (ex. 1234):')
    size = raw_input()
    try:
        size = int(size)
    except Exception:
        msg('Please send a number.')
        return False

    if size < 8 or size > 1024:
        msg('Size must be 8 ~ 1024.')
        return False

    msg('Send the binary in hex string:')
    data = sys.stdin.read(size*2)
    data = re.sub('\s+', '', data)
    data = binascii.unhexlify(data)
    f.write(data)
    f.flush()
    return True


def main():
    msg('Now send your program.bin')
    with tempfile.NamedTemporaryFile(mode='wb') as f:
        if get(f):
            run(f.name)


if __name__ == '__main__':
    sys.stderr = open('/dev/null', 'w')
    main()
