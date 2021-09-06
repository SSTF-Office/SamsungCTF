#!/usr/bin/env python
import requests
import json

APICHECK_KEY = '827f5e53ae260a44f2b444320230b8f6776ea0e63566e54f3fd7acc31dda9e6e'
URL = 'https://research.samsung.com/sctf2018/api/apikeycheck'

def check_user():
    apikey = input("Give me your apikey: ")
    data = {
            'apikey' : apikey,
            'apicheck_key' : APICHECK_KEY,
    }

    r = requests.post(URL, data=data)
    res = json.loads(r.text)

    if res['result'] == "success" and res['code'] == 200:
        return res['username']

    return
