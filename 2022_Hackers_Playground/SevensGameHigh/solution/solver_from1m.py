from pwn import *
import sys
import time

STR_SETTING = "Select the settings menu you want to change, or input any other key to go back into the main menu: \n"
STR_MENU = "Select the menu: \n"
STR_WELCOME = "Welcome to Seven's Game for High Roller!\n"
MAX_VALUE = 0

context.log_level = 'error'

trial = 0
success1 = 0
success2 = 0

start = time.time()
while 1:
    print(trial, end=" ")
    trial += 1

    # Phase 1: 100,000 to 1,000,000
    # Total free game triggers has to under 16 or the game may be never halted.
    p = process("./seven2")
    #p = remote("sevensgamehigh.sstf.site", 7777)
    p.sendline("2\n1000000\n5\n4\n2\n6\n0\n")
    p.recvuntil(STR_MENU)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)

    total_play = 0
    total_free = 0
    money = 0

    while 1:
        p.recvuntil(STR_MENU)
        p.sendline("1")

        print(".", end="")
        theresult = p.recvline(timeout=1)
        if (theresult == b''):
            print("Timeout!")
        if b"Your money is below" in theresult:
            p.close()
            #sys.exit()
            break
        else:
            moneyresult = theresult
        total_play += 1
        money = int(moneyresult.decode("latin-1").split("SCORE")[1].strip().replace(",", ""))
        p.recvuntil("+-------+-------+-------+\n")
        p.recvuntil("+-------+-------+-------+\n")
        result = p.recvline(timeout=1)
        if b"Win the Bonus Game!" in result:
            total_free += 1
            result = (p.recvuntil(STR_WELCOME))
            count = int(result.decode("latin-1").split("FREE PLAY : ")[-1].split("/ ")[1].split(" ")[0])
            money = int(result.decode("latin-1").split("SCORE")[-1].split("\n")[0].strip().replace(",", ""))
            result2 = (len(result.decode("latin-1")) - len(result.decode("latin-1").replace("Win the Bonus Game!", ""))) / len("Win the Bonus Game!")
            total_play += count
            total_free += result2

        elif b"Change the Bonus Game Combination!" in result:
            p.sendline("1")
        else:
            pass

        #MAX_VALUE = money if money > MAX_VALUE else MAX_VALUE
        #print("{:15,d}".format(1000000000))
        if (money > 1000000000):
            success1 += 1
            p.close()
            print("\n[%f]\nPhase 1 Done\nPhase 1 : %d / %d (%f%%)\nPhase 2 : %d (%f%% from Phase 1, %f%% total)" % (time.time(), success1, trial, success1 / trial * 100, success2, success2 / success1 * 100, success2 / trial * 100))
            print("Execution time", (time.time() - start) / trial)
            break
    #print("{:15,d}".format(MAX_VALUE), end="")
