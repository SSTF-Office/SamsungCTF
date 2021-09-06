from pwn import *
from random import choice
from ctypes import c_uint32
import signal

#context.log_level = 'error'
context.arch = 'amd64'

flag = 'SCTF{45m_d1545m_f0r3v3r}'
STAGE = 10

TIME_LIMIT = 120
TOO_SLOW = '''
  _______                _                   _____
 |__   __|              | |                _|  __ \\
    | | ___   ___    ___| | _____      __ (_) |__) |
    | |/ _ \\ / _ \\  / __| |/ _ \\ \\ /\\ / /   |  ___/
    | | (_) | (_) | \\__ \\ | (_) \\ V  V /   _| |
    |_|\\___/ \\___/  |___/_|\\___/ \\_/\\_/   (_)_|
'''

def handler(signum, frame):
    print TOO_SLOW
    exit(0)
    return

form = '''cmp eax, {}
je good
mov eax, 0
mov eax, [eax]
good:
{}
'''

epilogue = '''cmp eax, {}
je good
mov eax, 0
mov eax, [eax]
good:
mov rax, 60
mov rdi, 0
syscall'''

def make_rand_op(n=3):
    ops = {'+': 'add eax, {}',
           '-': 'sub eax, {}',
           '*': 'mov edx, {}\nmul edx'}
    nums = [randint(0, 2**32) for i in xrange(n)]
    ops_res = [choice(ops.keys()) for i in xrange(n-1)]
    ins = [ops[i] for i in ops_res]
    ins = ['mov eax, {}'] + ins

    for i in xrange(len(ins)):
        ins[i] = ins[i].format(nums[i])

    num_res = nums[0]
    for i, v in enumerate(ops_res):
        num_res = eval('{} {} {}'.format(num_res, ops_res[i], nums[i+1]))

    num_res = c_uint32(num_res).value

    return '\n'.join(ins), num_res

def make_p_set():
    res = []
    nv = randint(0, 2**32)
    for i in xrange(10):
        sv = nv
        code, nv = make_rand_op(randint(3, 6))
        res.append(form.format(sv, code))
    res.append(epilogue.format(nv))

    res[0] = res[0].split('good:\n')[-1]
    res = map(asm, res)

    return res

def main():
    print 'Each element of list is base64-encoded x64 machine code'
    print 'You can concatenate them and execute it'
    print 'Make a longest code chain without any segfault'
    print
    sleep(5)

    signal.signal(signal.SIGALRM, handler)
    signal.alarm(TIME_LIMIT)

    for i in xrange(STAGE):
        print 'stage ({}/{})'.format(i+1, STAGE)
        res = make_p_set()
        answer = sha256sumhex(''.join(res))

        res = map(b64e, res)
        shuffle(res)
        print res

        inp = raw_input('Give me sha256 hexsum of answer(code bytes): ').rstrip()
        if not inp == answer:
            print 'wrong :P'
            exit()

    print 'Good Job!'
    print flag

if __name__ == '__main__':
    main()
