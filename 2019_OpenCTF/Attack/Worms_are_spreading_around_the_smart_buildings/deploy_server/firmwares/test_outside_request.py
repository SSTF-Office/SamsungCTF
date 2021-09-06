import requests

try:
    r = requests.get('http://10.8.0.5:8080/', timeout=0.001)
    print("Status 1: {}".format(r.status_code))
except:
    print("Request 1 failed")

try:
    r = requests.get('http://10.8.0.5:80/', timeout=0.001)
    print("Status 2: {}".format(r.status_code))
except:
    print("Request 2 failed")

try:
    r = requests.get('http://www.gazeta.pl', timeout=0.001)
    print("Status 3: {}".format(r.status_code))
except:
    print("Request 3 failed")

try:
    r = requests.get('http://www.gazeta.pl:8080', timeout=0.001)
    print("Status 4: {}".format(r.status_code))
except:
    print("Request 4 failed")
