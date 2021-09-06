## Auth Code

### Description

I extracted an auth code generator for a CTF.

You may need some cryptographic knowledge to pwn it.

Download: [AuthCode.zip](deploy/download/authcode.zip)

Server: `nc authcode.sstf.site 1337`

### Deploy
```
cd deploy
./deploy.sh
echo "1" | ./deploy.sh
echo "3" | ./deploy.sh
```

