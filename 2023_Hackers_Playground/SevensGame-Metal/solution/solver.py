from pwn import *
import sys
import time
import random
import tensorflow as tf
from tensorflow.keras import datasets, layers, models
import os
import numpy as np

tf.get_logger().setLevel('ERROR')
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
context.log_level = 'error'

STR_SETTING = "Select the settings menu you want to change, or input any other key to go back into the main menu: \n"
STR_MENU = "Select the menu: \n"

STR_DIAMOND = b'\x1b[33m \xe2\x96\x97\xe2\x96\x86\xe2\x96\x86\xe2\x96\x86\xe2\x96\x96 \x1b[37m'

BATCH_SIZE = 10

def int2digitlist(x, digit=8):
    return list(map(int, (("{0:0%db}"%digit).format(x))))

def learn(data, result, proc):
    input1 = layers.Input(shape=(20,))
    dense1 = layers.Dense(20, activation='sigmoid', kernel_initializer='glorot_normal', use_bias=False)
    normal2 = layers.Dense(20, activation='linear')
    dense2 = layers.Dense(18, activation='sigmoid', kernel_initializer='glorot_normal', use_bias=False)

    m = models.Model([input1], dense2(normal2(dense1((input1)))))
    normal2.set_weights([np.identity(20) ,np.ones(20) * (-0.5)])
    normal2.trainable = False

    m.compile(
    optimizer=tf.keras.optimizers.RMSprop(learning_rate=0.001),
    loss=tf.keras.losses.BinaryCrossentropy(),
    metrics=[tf.keras.metrics.MeanSquaredError(), tf.keras.metrics.BinaryAccuracy()],
    )

    trainx = []
    trainy = []
    for d in data:
        nums = d
        trainx.append(int2digitlist(int(nums[0]), 20))
        trainy.append(int2digitlist(int(nums[1]), 18))

    seed_new = result[0] | (result[1] << 6) | (result[2] << 12)
    # Because the SEED after spining is multiple of 3
    if (seed_new % 3 == 0):
        seed_new = seed_new // 3
    elif ((seed_new | (1 << 18)) % 3 == 0):
        seed_new = (seed_new | (1 << 18)) // 3
    else:
        seed_new = (seed_new | (1 << 19)) // 3
    lucky = int2digitlist(seed_new, 18)

    # NOTE: repeat to fit by 100 epochs
    for _ in range(15):
        accuracy = (m.fit(np.array(trainx) - 0.5, (np.array(trainy)), epochs=100, verbose=0).history["binary_accuracy"][-1])
        #print(accuracy)
        proc.sendline("1")
        proc.recvuntil(STR_MENU)
    print("Accuracy %f%%" % (accuracy * 100))

    # NOTE: split the train set to do in the low performance PC
    tx1 = list(range(2**19))
    tx1 = np.array(list(map(lambda d: int2digitlist(d, 20), tx1)))
    ty1 = m.predict(tx1 - 0.5)
    proc.sendline("1")
    proc.recvuntil(STR_MENU)
    tx2 = list(range(2**19, 2**20))
    tx2 = np.array(list(map(lambda d: int2digitlist(d, 20), tx2)))
    ty2 = m.predict(tx2 - 0.5)
    tx = np.concatenate((tx1, tx2))
    ty = np.concatenate((ty1, ty2))

    print("SEED of JACKPOT =", int("".join(map(str,lucky)), 2))

    candidates = []
    result = (np.round(ty))
    for i in range(len(tx)):
        if np.equal(np.round(result[i]), lucky).all():
            candidates.append(tx[i])
    proc.sendline("1")
    proc.recvuntil(STR_MENU)

    return candidates

trial = 0

start = time.time()
while 1:
    seed = 0
    diamonds = [-1, -1, -1]
    graphs = {}
    total_spin = 0
    total_seed = 0

    trial += 1
    print()
    print("Trial #%d" % trial)
    print("Collecting the train set data")

    need_change_seed = False
    #p = process(["./seven3", "1000", "1000", "1"])
    p = remote("sevensgamemetal.sstf.site", 7777)
    p.sendline("5\n5\n3\n2\n8\n%d\n0\n" % BATCH_SIZE)
    p.recvuntil(STR_MENU)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)
    p.recvuntil(STR_SETTING)

    p.recvuntil(STR_MENU)
    count1 = 0
    count2 = 0
    while 1:
        break_flag = False
        if need_change_seed: # Fall in loop
            seed_random = random.randint(0, 2 ** 18 - 1) | (0b11 << 18)
            p.sendline("5\n4\n%d\n0\n" % seed_random)
            p.recvuntil(STR_SETTING)
            p.recvuntil(STR_SETTING)
            p.recvuntil(STR_MENU)
            seed = seed_random
            need_change_seed = False
        p.sendline("1")
        total_spin += BATCH_SIZE

        allresult = p.recvuntil(STR_MENU, timeout=1)
        
        theresults = allresult.split(b" SCORE")
        for k in range(1, BATCH_SIZE + 1):
            try:
                theresult = theresults[k]
            except:
                print(theresults)
            lines = theresult.split(b"\n")
            
            cells = lines[3].split(b"|")

            result = list(map(lambda x: ((int(x) + 1) // 2) % 64, lines[7].decode("utf8").split("targets : ")[1].split(", symbol_count")[0].split(", ")))
            for i in range(3):
                if (STR_DIAMOND in cells[1+i]):
                    diamonds[i] = result[i]

            seed_new = result[0] | (result[1] << 6) | (result[2] << 12)
            # Because the SEED after spining is multiple of 3
            if (seed_new % 3 == 0):
                seed_new = seed_new // 3
            elif ((seed_new | (1 << 18)) % 3 == 0):
                seed_new = (seed_new | (1 << 18)) // 3
            else:
                seed_new = (seed_new | (1 << 19)) // 3

            if seed_new * 3 in graphs:
                graphs[seed] = seed_new
                need_change_seed = True
                seed = seed_new * 3
                count1 += 1
            else:
                graphs[seed] = seed_new
                seed = seed_new * 3
                count2 += 1
            

            if (total_spin >= 400):
                if (diamonds[0] == -1 or diamonds[1] == -1 or diamonds[2] == -1):
                    print("Try again - No JACKPOT at this time")
                    break_flag = True
                    break
                p.sendline("5\n8\n1\n0\n")
                p.recvuntil(STR_MENU)
                data = []
                for k in graphs:
                    data.append((k, graphs[k]))
                result = learn(data, diamonds, p)
                print("Diamond's reel index :", diamonds)
                print("# of train set :", len(data))
                print("# of spin with loop :", count1)
                print("# of spin without loop :", count2)
                if (len(result) == 0):
                    print("Try again - No candidate of SEED giving JACKPOT")
                    break_flag = True
                    break
                print("See candidates of SEED giving JACKPOT")
                for r in result:
                    seed = (int("".join(map(str,r)), 2))
                    print(seed)
                for r in result:
                    seed = (int("".join(map(str,r)), 2))
                    print("Try SEED :", seed)
                    p.sendline("5\n4\n%d\n0\n1\n" % seed)
                    p.recvuntil(STR_MENU)
                    spin_result = (p.recvuntil(STR_MENU).decode("utf8"))
                    print(spin_result)
                    if "result_code : 15" in spin_result:
                        print("Find the SEED of Jackpot!")
                        p.sendline("2\n10000\n")
                        p.recvuntil(STR_MENU)
                        p.sendline("5\n4\n%d\n0\n1\n" % seed)
                        p.recvuntil(STR_MENU)
                        p.recvuntil(STR_MENU)
                        p.sendline("3\n1\n0\n")
                        print("##########BUY FLAG##########")
                        print(p.recvuntil(STR_MENU).decode("utf8"))
                        sys.exit()
                break_flag = True
                break
        if break_flag:
            break

