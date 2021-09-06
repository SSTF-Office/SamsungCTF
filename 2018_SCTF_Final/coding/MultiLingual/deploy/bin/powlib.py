import sys
import random
import hashlib


def POW(difficulty):
    prefix = str(random.randrange(1000000))
    sys.stdout.write('As a proof-of-work, give me an input that starts with '
                     '{0} and which makes its SHA256 hash to start with {1} '
                     'NULL bytes\n'.format(prefix, difficulty / 2.0))
    sys.stdout.write("pow: ")
    sys.stdout.flush()
    data = raw_input().strip()
    hd = hashlib.sha256(prefix + data).hexdigest()
    find = '0'*difficulty
    if not hd.startswith(find):
        sys.stdout.write('SHA256({0}) does not start with {1}\n'
                         .format(prefix+data, find))
        sys.stdout.flush()
        exit(0)
