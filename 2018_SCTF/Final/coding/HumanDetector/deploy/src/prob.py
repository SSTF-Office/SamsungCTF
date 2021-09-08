#!/usr/bin/python3
import random
import os
import sys
import cv2
import zlib
import numpy as np
from PIL import Image
import datetime

CHAR_NUM = 100
SUCCESS = CHAR_NUM * 0.80
IMAGE_WIDTH = 60
IMAGE_HEIGHT = 100

os.chdir("/home/humandetector")
FILE_LIST = "image_list.txt"
FLAG = os.environ["FLAG"]

def puts(msg):
    if not isinstance(msg, bytes):
        msg += "\n"
        msg = msg.encode()
    sys.stdout.buffer.write(msg)
    sys.stdout.flush()


def write(msg):
    if not isinstance(msg, bytes):
        msg = msg.encode()
    sys.stdout.buffer.write(msg)
    sys.stdout.flush()


def show_images():
    user = 'user'

    puts("[+] Hi %s, only bots can pass this test." % (user))
    puts("[+] You should pass %d out of %d characters." % (SUCCESS, CHAR_NUM))
    puts("[+] creating a char image ...")
    with open(FILE_LIST, "r") as f:
        fnames = f.read().splitlines()
        fnames = random.sample(fnames, CHAR_NUM)

    labels = np.array(list(map(lambda x: os.path.basename(x).split("_")[0],
                               fnames)))
    new_img = np.zeros((IMAGE_HEIGHT, IMAGE_WIDTH * CHAR_NUM, 3))
    for idx, fname in enumerate(fnames):
        new_img[:,
                idx*IMAGE_WIDTH:(idx+1)*IMAGE_WIDTH,
                :] = cv2.imread(fname)
    new_img = cv2.imencode(".png", new_img)[1].tobytes()
    img_compressed = zlib.compress(new_img)

    puts("[+] done.")
    puts("[+] sending zlib compressed image data (size: %d)" % (len(img_compressed)))
    input("(type 'enter' to start)")
    write(img_compressed)
    puts("[+] now, you should predict the labels.")
    puts("[+] the labels should be sperated by ','")
    puts("[+] example: A,B,C,D,E")
    puts("give me the labels: ")
    predicted = input()
    try:
        predicted = np.array(predicted.split(","))
        success = sum(predicted == labels)
        if success > SUCCESS:
            puts("[+] congratulations! success rate (%d/%d)!" % (success, CHAR_NUM))
            puts("[+] You are the certified bot. this is your flag: %s" % (FLAG))

        else:
            puts("[-] Nope. success rate (%d/%d)"  % (success, CHAR_NUM))
    except:
        puts("[*] ERROR! please send with the correct format.")



def show_logo():
    logo = '''
                                                                                               
        ,--,                        ____                          ,--.                         
      ,--.'|                      ,'  , `.   ,---,              ,--.'|                         
   ,--,  | :         ,--,      ,-+-,.' _ |  '  .' \         ,--,:  : |                         
,---.'|  : '       ,'_ /|   ,-+-. ;   , || /  ;    '.    ,`--.'`|  ' :                         
|   | : _' |  .--. |  | :  ,--.'|'   |  ;|:  :       \   |   :  :  | |                         
:   : |.'  |,'_ /| :  . | |   |  ,', |  '::  |   /\   \  :   |   \ | :                         
|   ' '  ; :|  ' | |  . . |   | /  | |  |||  :  ' ;.   : |   : '  '; |                         
'   |  .'. ||  | ' |  | | '   | :  | :  |,|  |  ;/  \   \'   ' ;.    ;                         
|   | :  | ':  | | :  ' ; ;   . |  ; |--' '  :  | \  \ ,'|   | | \   |                         
'   : |  : ;|  ; ' |  | ' |   : |  | ,    |  |  '  '--'  '   : |  ; .'                         
|   | '  ,/ :  | : ;  ; | |   : '  |/     |  :  :        |   | '`--'                           
;   : ;--'  '  :  `--'   \;   | |`-'      |  | ,'        '   : |                               
|   ,/      :  ,      .-./|   ;/          `--''          ;   |.'                               
'---'        `--`----'    '---',----,                    '---'   ,----,                        
                             ,/   .`|                          ,/   .`|  ,----..               
    ,---,        ,---,.    ,`   .'  :   ,---,.  ,----..      ,`   .'  : /   /   \  ,-.----.    
  .'  .' `\    ,'  .' |  ;    ;     / ,'  .' | /   /   \   ;    ;     //   .     : \    /  \   
,---.'     \ ,---.'   |.'___,/    ,',---.'   ||   :     :.'___,/    ,'.   /   ;.  \;   :    \  
|   |  .`\  ||   |   .'|    :     | |   |   .'.   |  ;. /|    :     |.   ;   /  ` ;|   | .\ :  
:   : |  '  |:   :  |-,;    |.';  ; :   :  |-,.   ; /--` ;    |.';  ;;   |  ; \ ; |.   : |: |  
|   ' '  ;  ::   |  ;/|`----'  |  | :   |  ;/|;   | ;    `----'  |  ||   :  | ; | '|   |  \ :  
'   | ;  .  ||   :   .'    '   :  ; |   :   .'|   : |        '   :  ;.   |  ' ' ' :|   : .  /  
|   | :  |  '|   |  |-,    |   |  ' |   |  |-,.   | '___     |   |  ''   ;  \; /  |;   | |  \  
'   : | /  ; '   :  ;/|    '   :  | '   :  ;/|'   ; : .'|    '   :  | \   \  ',  / |   | ;\  \ 
|   | '` ,/  |   |    \    ;   |.'  |   |    \'   | '/  :    ;   |.'   ;   :    /  :   ' | \.' 
;   :  .'    |   :   .'    '---'    |   :   .'|   :    /     '---'      \   \ .'   :   : :-'   
|   ,.'      |   | ,'               |   | ,'   \   \ .'                  `---`     |   |.'     
'---'        `----'                 `----'      `---`                              `---'       
                                                                                               
'''
    puts(logo)

show_logo()
show_images()
