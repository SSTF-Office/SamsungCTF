import subprocess
import random
import string
import os

flag = "SCTF{dVWi_JFJRyZp_wRQltegoc}"

binary = raw_input("Base64 encoded binary: ")

#encoded_bin = "./encoded.from.client"
#with open(encoded_bin, "wb") as f:
#    f.write(binary)

try:
    binary = binary.strip().decode("base64")
    print "Base64 Decoding done"
except:
    print ("Base64 Decoding Fail")
    exit()

letters = string.uppercase + string.lowercase + string.digits
bin_name = "/tmp/" + "".join([random.choice(letters) for _ in range(16)])
#print bin_name
res_name = bin_name + "_res.txt"

with open(bin_name, "wb") as f:
    f.write(binary)

cmd1 = "/home/code/loader " + bin_name + " /home/code/test_input1.txt " + res_name
cmd2 = "/home/code/check.sh /home/code/test_input1.txt " + res_name
os.chmod(bin_name, 0544)

subprocess.check_output(cmd1.split())
res = subprocess.check_output(cmd2.split()).strip()
if res == "true":
    print ("Congratz! Your flag is " + flag)
else:
    print (res)

os.remove(bin_name)
