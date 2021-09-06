#!/usr/bin/python2
import sys
import time
import signal
from powlib import POW
from executor import execute_arch, TARGETS, Error


def write(x, delay=0):
    sys.stdout.write(x)
    sys.stdout.flush()
    time.sleep(delay)


def main():
    write('''\
      ___           ___           ___       ___
     /\__\         /\__\         /\__\     /\  \          ___
    /::|  |       /:/  /        /:/  /     \:\  \        /\  \\
   /:|:|  |      /:/  /        /:/  /       \:\  \       \:\  \\
  /:/|:|__|__   /:/  /  ___   /:/  /        /::\  \      /::\__\\
 /:/ |::::\__\ /:/__/  /\__\ /:/__/        /:/\:\__\  __/:/\/__/
 \/__/~~/:/  / \:\  \ /:/  / \:\  \       /:/  \/__/ /\/:/  /
       /:/  /   \:\  /:/  /   \:\  \     /:/  /      \::/__/
      /:/  /     \:\/:/  /     \:\  \    \/__/        \:\__\\
     /:/  /       \::/  /       \:\__\                 \/__/
     \/__/         \/__/         \/__/
      ___                   ___           ___           ___           ___           ___
     /\__\      ___        /\__\         /\  \         /\__\         /\  \         /\__\\
    /:/  /     /\  \      /::|  |       /::\  \       /:/  /        /::\  \       /:/  /
   /:/  /      \:\  \    /:|:|  |      /:/\:\  \     /:/  /        /:/\:\  \     /:/  /
  /:/  /       /::\__\  /:/|:|  |__   /:/  \:\  \   /:/  /  ___   /::\~\:\  \   /:/  /
 /:/__/     __/:/\/__/ /:/ |:| /\__\ /:/__/_\:\__\ /:/__/  /\__\ /:/\:\ \:\__\ /:/__/
 \:\  \    /\/:/  /    \/__|:|/:/  / \:\  /\ \/__/ \:\  \ /:/  / \/__\:\/:/  / \:\  \\
  \:\  \   \::/__/         |:/:/  /   \:\ \:\__\    \:\  /:/  /       \::/  /   \:\  \\
   \:\  \   \:\__\         |::/  /     \:\/:/  /     \:\/:/  /        /:/  /     \:\  \\
    \:\__\   \/__/         /:/  /       \::/  /       \::/  /        /:/  /       \:\__\\
     \/__/                 \/__/         \/__/         \/__/         \/__/         \/__/
\n\n\n''', 4)
    write('Can you tell me a sentence that can be translated as 4 different meanings?\n')
    write('Your sentence should be translated as follow.\n\n'\
          'if X86 then "Hi! I like X86."\n'\
          'if X86_64 then "Hi! I like X86_64."\n'\
          'if AArch64 then "Hi! I like aarch64."\n'\
          'if Mips(Little Endian, 32bit) then "Hi! I like mips."\n\n')
    write('There are some more rules:\n'\
          '- Your sentence will be locate at 0x1000000.\n'\
          '- Valid system call:  [SYS_WRITE, SYS_EXIT].\n'\
          '- Valid Memory space: [0x1000000 ~ 0x1000fff](RWE).\n'\
          '- If program counter be end of your code, it immediately exit.\n'
          '\n\n', 2)

    write('What\'s your sentence? (base64 encoded, 256bytes max): ')
    code = sys.stdin.readline().decode('base64')
    if len(code) > 0x100:
        write("Too looong")
        exit(0)

    try:
        write('Good Luck!\n', 4)
        flag = 'SCTF{N0w_Y0u_C4n_Sp34k_Ev3ry7h1ng}'
        outputs = [execute_arch(code, t).strip() for t in TARGETS]
        if all([o == 'Hi! I like {}.'.format(t)
                for o, t in zip(outputs, TARGETS)]):
            write('Wow! You\'re multilingual!\n', 1)
            write('This is my response: {}!\n'.format(flag))
            exit(0)
    except Error:
        pass
    except Exception as e:
        write(e)
        exit(0)

    write('You\'re not multilingual.\n', 1)
    write('Get out of here!\n')
    exit(0)


if __name__ == '__main__':
    signal.alarm(60)
    sys.stderr = open('/dev/null', 'w')
    main()
