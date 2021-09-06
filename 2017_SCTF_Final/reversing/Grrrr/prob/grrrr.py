import os
import sys

key = "Grumpy makes me suck!"
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'Usage grrrr.py [flag]'
    else:
        flag = sys.argv[1]
        if len(flag) == 27:
            if ord(flag[0]) ^ 20 == ord(key[0]):
                if ord(flag[1]) ^ 49 == ord(key[1]):
                    if ord(flag[2]) ^ 33 == ord(key[2]):
                        if ord(flag[3]) ^ 43 == ord(key[3]):
                            if ord(flag[4]) ^ 11 == ord(key[4]):
                                if ord(flag[5]) ^ 79 == ord(key[5]):
                                    if ord(flag[6]) ^ 82 == ord(key[6]):
                                        if ord(flag[7]) ^ 24 == ord(key[7]):
                                            if ord(flag[8]) ^ 12 == ord(key[8]):
                                                if ord(flag[9]) ^ 27 == ord(key[9]):
                                                    if ord(flag[10]) ^ 28 == ord(key[10]):
                                                        if ord(flag[11]) ^ 44 == ord(key[11]):
                                                            if ord(flag[12]) ^ 67 == ord(key[12]):
                                                                if ord(flag[13]) ^ 89 == ord(key[13]):
                                                                    if ord(flag[14]) ^ 82 == ord(key[14]):
                                                                        if ord(flag[15]) ^ 127 == ord(key[15]):
                                                                            if ord(flag[16]) ^ 30 == ord(key[16]):
                                                                                if ord(flag[17]) ^ 70 == ord(key[17]):
                                                                                    if ord(flag[18]) ^ 83 == ord(key[18]):
                                                                                        if ord(flag[19]) ^ 28 == ord(key[19]):
                                                                                            if ord(flag[20]) ^ 20 == ord(key[20]):
                                                                                                if ord(flag[21]) ^ 24 == ord(key[0]):
                                                                                                    if ord(flag[22]) ^ 68 == ord(key[1]):
                                                                                                        if ord(flag[23]) ^ 7 == ord(key[2]):
                                                                                                            if ord(flag[24]) ^ 31 == ord(key[3]):
                                                                                                                if ord(flag[25]) ^ 2 == ord(key[4]):
                                                                                                                    if ord(flag[26]) ^ 4 == ord(key[5]):
                                                                                                                        print "Congratulations!"
