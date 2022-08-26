$ cd ./deploy
$ ./build.sh
$ ./run.sh

You might want to check if it's working:

$ ./check.py
[+] Connecting to localhost on port 8042: Done
[*] sstf@localhost:
    Distro    Ubuntu 20.04
    OS:       linux
    Arch:     amd64
    Version:  5.11.0
    ASLR:     Enabled
[*] Working directory: '/tmp/tmp.iXKNboBKHI'
[*] Uploading './../exploit/x' to '/tmp/tmp.iXKNboBKHI/x'
[*] Uploading './../exploit/x.so' to '/tmp/tmp.iXKNboBKHI/x.so'
[+] Opening new channel: 'chmod +x ./x': Done
[+] Opening new channel: 'chmod +x ./x.so': Done
[+] Opening new channel: 'ln -s /home/pwnkit/flag .': Done
[+] Opening new channel: 'ln -s /home/pwnkit/pkexec .': Done
[!] OK. (found: b'SCTF{pony pwnie pol pol jjak}')
[*] Closed connection to 'localhost'
[*] Closed SSH channel with localhost
[*] Closed SSH channel with localhost
[*] Closed SSH channel with localhost
[*] Closed SSH channel with localhost

If you can see "OK. ..", you are good to launch!