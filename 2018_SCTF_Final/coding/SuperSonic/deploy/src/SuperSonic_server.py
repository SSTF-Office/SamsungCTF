from random import choice
from time import sleep, ctime, time
from solve import solve

import signal
import os

TIME_LIMIT = 20
STAGE_NUM = 2000
INTRO_MSG = '''
--------------------------------------------
This is a really simple challenge! :P

I will give you {} problems,
but you can solve them with a given solver!
Too easy, isn't it?
--------------------------------------------
'''.format(STAGE_NUM)
USERNAME = 'Anonymous'

TOO_SLOW = '''
  _______                _                   _____
 |__   __|              | |                _|  __ \\
    | | ___   ___    ___| | _____      __ (_) |__) |
    | |/ _ \\ / _ \\  / __| |/ _ \\ \\ /\\ / /   |  ___/
    | | (_) | (_) | \\__ \\ | (_) \\ V  V /   _| |
    |_|\\___/ \\___/  |___/_|\\___/ \\_/\\_/   (_)_|
'''

start = 0
stage = 0

def check_user(apikey):
    return {'result': 'success', 'code': 200, 'username': 'dummy'}

def log(string):
    global start, USERNAME

    if 'SOCAT_PEERADDR' not in os.environ:
        os.environ['SOCAT_PEERADDR'] = 'Unknown'

    end = time()
    with open('/tmp/log/log', 'a') as f:
        f.write('{} |||| {} |||| {} |||| {} |||| {} |||| {}\n'.format(USERNAME, stage,
            os.environ['SOCAT_PEERADDR'], ctime(end), end-start, string))

    return

def alarm_handler(signum, frame):
    log('Slow')

    print TOO_SLOW
    exit(2)
    return

def main():
    global start, stage

    print INTRO_MSG
    sleep(3)
    start = time()

    cand = 'ab'
    for stage in xrange(1, STAGE_NUM+1):
        print
        print '[*] stage ({}/{})'.format(stage, STAGE_NUM)
        print

        inp = ''.join(choice(cand) for stage in xrange(10000))

        print 'input : {}'.format(inp)
        try:
            answer_huh = raw_input('give me your answer : ')
        except EOFError as e:
            log('Quit')
            exit(3)


        if answer_huh == solve(inp):
            print
            print '[+] Good Job!'
        else:
            print
            print "[-] Nooooooooo! Why don't you use my solver?"
            log('Fail')

            exit(1)

    print 'Great! Now you can get the flag'
    with open('flag') as f:
        print f.read()

    log('Success')

    return

if __name__ == "__main__":
    apikey = 'dummy'
    res = check_user(apikey)

    if res['result'] == 'success' and res['code'] == 200:
        print 'Hi, {}. You successfully verified'.format(res['username'])
        USERNAME = res['username']
    else:
        print 'Who are you?'
        exit(1)

    signal.signal(signal.SIGALRM, alarm_handler)
    signal.alarm(TIME_LIMIT+3)

    main()
