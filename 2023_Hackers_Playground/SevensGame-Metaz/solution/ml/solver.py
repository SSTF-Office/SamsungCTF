from pwn import *
import sys
import time
import random
import tensorflow as tf
from tensorflow.keras import datasets, layers, models
import os
import numpy as np
import datetime
import json

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

    print(str(datetime.datetime.now()), "start learn")
    # NOTE: repeating to fit by 100 epochs
    for _ in range(15):
        accuracy = (m.fit(np.array(trainx) - 0.5, (np.array(trainy)), epochs=100, verbose=0).history["binary_accuracy"][-1])
        #print(accuracy)
        proc.sendline("1")
        proc.recvuntil(STR_MENU)
    print("Accuracy %f%%" % (accuracy * 100))

    # NOTE: split the train set to do in the low performance PC
    print(str(datetime.datetime.now()), "start predict")
    ty = m.predict(tx - 0.5)
    proc.sendline("1")
    proc.recvuntil(STR_MENU)

    print(str(datetime.datetime.now()), "start checking")
    candidates = []
    result = (np.round(ty))
    for i in range(len(tx)):
        y_new = (int("".join(map(lambda x: str(int(x)), result[i])), 2)) * 3
        x_new = (int("".join(map(lambda x: str(int(x)), tx[i])), 2))
        if x_new == y_new:
            candidates.append(tx[i])
    print(str(datetime.datetime.now()), "end checking")


    proc.sendline("1")
    proc.recvuntil(STR_MENU)

    return candidates

trial = 0

with open("numbers.txt", "r") as fp:
    tx = json.load(fp)
tx = np.array(list(map(lambda d: int2digitlist(d, 20), tx)))

start = time.time()
while 1:
    seed = 0
    graphs = {}
    total_spin = 0
    total_seed = 0
    seed_change = 0

    trial += 1
    print()
    print("Trial #%d" % trial)
    print(str(datetime.datetime.now()), "start collect")
    print("Collecting the train set data")

    need_change_seed = False
    #p = process(["./seven3", "2000", "100", "0"])
    p = remote("sevensgamemetaz76775d33.sstf.site", 7777)
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
            seed_change += 1
            #print("%d" % seed_change)
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
            

            if (total_spin >= 900):
                p.sendline("5\n8\n1\n0\n")
                p.recvuntil(STR_MENU)
                data = []
                for k in graphs:
                    data.append((k, graphs[k]))
                result = learn(data, None, p)
                print("# of train set :", len(data))
                print("# of spin with loop :", count1)
                print("# of spin without loop :", count2)
                if (len(result) == 0):
                    print("Try again - No candidate of a fixed point")
                    break_flag = True
                    break
                print("See candidates of a fixed point")
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
                    if "result_code : 3" in spin_result:
                        print("Confirm the result of BBB!")
                    else:
                        continue
                    spin_result_targets = spin_result.split("targets : ")[1].split(", symbol_count")[0]
                    p.sendline("1\n")
                    spin_result = (p.recvuntil(STR_MENU).decode("utf8"))
                    spin_result_targets_new = spin_result.split("targets : ")[1].split(", symbol_count")[0]
                    if spin_result_targets == spin_result_targets_new:
                        print("Confirm the loop!")
                        p.sendline("2\n10000\n")
                        p.recvuntil(STR_MENU)
                        p.sendline("5\n4\n%d\n7\n337\n0\n1\n1\n1\n" % seed)
                        p.recvuntil(STR_MENU)
                        p.recvuntil(STR_MENU)
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

