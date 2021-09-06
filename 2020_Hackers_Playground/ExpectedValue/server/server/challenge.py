import base64
import tempfile
import subprocess
import os
import pathlib
import shutil

try:
	shellcode = input("Base64 encoded shellcode > ")
	shellcode = base64.b64decode(shellcode, validate=True)
except Exception as e:
	print("[Err]Invalid intput.", e)
	exit()

if len(shellcode) > 128:
	print ("[Err]Your shellcode is too long(max 128 bytes).")
	exit()
elif not shellcode.startswith(b"\x55\x48\x89\xe5"):
	print ("[Err]The shellcode should start with function prologue(\"0x554889e5\").")
	exit()
elif not shellcode.endswith(b"\xc9\xc3"):
	print ("[Err]The shellcode should end with function prologue(\"0xc9c3\").")
	exit()
elif b"\x00" in shellcode or b"\xcd" in shellcode or b"\x0f" in shellcode or b"\x95" in shellcode:
	print("[Err]The shellcode cannot contain 0x00, 0xcd, 0x0f or 0x95.")
	exit()

curdir = pathlib.Path(__file__).parent.absolute()
right_answer = open(os.path.join(curdir, "right_answers")).read()

tempdir = tempfile.TemporaryDirectory(prefix = "tmp_")

#print ("Using temporary directory " + tempdir.name)
os.chmod(tempdir.name, 711)
shutil.copyfile(os.path.join(curdir, "validate"), os.path.join(tempdir.name, "validate"))
os.chmod(os.path.join(tempdir.name, "validate"), 711)
with open(os.path.join(tempdir.name, "payload"), "wb") as f:
	f.write(shellcode)

try:
	result = subprocess.check_output(("chroot --userspec=challenger:challenger %s /validate payload"%tempdir.name).split(), timeout=3).decode()
	#result = subprocess.check_output(("chroot %s /validate payload"%tempdir.name).split(), timeout=3).decode()
except Exception as e:
	result = "nope."
	print (e)

tempdir.cleanup()

if (result == right_answer):
	print("Success! Here's the flag for you!")
	print(open(os.path.join(curdir, "flag")).read())
else:
	print("Wrong result. Try again.")
	#print (result)
