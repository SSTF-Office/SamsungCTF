#!/usr/bin/env python3
import sys
import os
from time import sleep
from quotes import *
from TCs import genTC
from TM import getInput
from time import strftime, localtime

FLAG = 'SCTF{Turing_Machine_is_v3ry_p0werful1}'
def write(string):
    sys.stdout.write(string)
    sys.stdout.flush()

def bye(string):
    write(str(colored.red(string)))
    exit(0)

def log(stage, line, res):
    LOG_ROOT = '/log'
    remote_addr = os.environ['REMOTE_HOST']
    if not os.path.exists(os.path.join(LOG_ROOT, remote_addr)):
        os.mkdir(os.path.join(LOG_ROOT, remote_addr))
    now = strftime('%y-%m-%d-%H:%M:%s', localtime())
    with open(os.path.join(LOG_ROOT, remote_addr, '%s-%d-%s'%(now, stage, res)), 'w') as f:
        f.write(line)

def chk(stage):
    sleep(1)
    write(infos[stage])
    try:
        TM, line = getInput()
    except:
        bye('Illegal input\n')

    tests = genTC[stage]()
    for i in range(len(tests)):
        progress = '{}/{}'.format(i+1, len(tests))
        write('\r{} {}'.format(colored.yellow('Testing'), colored.green(progress)))
        sleep(0.001)
        T_res, T_input = tests[i]
        try:
            if TM.run(T_input) != T_res:
                log(stage+1, line, 'FAIL')
                bye('\nlose...\n')
        except OutOfStepException:
            log(stage+1, line, 'OOS')
            bye('\nOut of step\n')
        except IllegalDirectionException:
            log(stage+1, line, 'IDE')
            bye('\nIllegal direction\n')
    log(stage+1, line, 'SUCC')
    write(str(colored.green('\npass!\n')))

def main():
    write(str(colored.green(start)))
    sleep(3)
    write('\n\n{} {}\n'.format(colored.red('Hi!'), colored.blue('Welcome!')))
    write('Welcome to the {}\n\n'.format(colored.yellow('Turing Competition!')))
    sleep(3)

    write('Build Your own {}!\n'.format(colored.red('TM')))
    write("Your TM {}.\nMaximum moves: {}\n".format(colored.magenta('MUST halt'), colored.red('100,000')))
    sleep(1)
    for stage in range(4):
        chk(stage)
        sleep(2)

    write('\n{} You are the {} of this game!\n'.format(colored.red('Wow!'), colored.yellow('winner')))
    sleep(1)
    write('This is Prize: {}\n'.format(colored.yellow(FLAG)))
    exit(0)

if __name__ == "__main__":
    sys.stderr.close()
    main()

