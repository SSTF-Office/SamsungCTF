# build
```
aarch64-linux-gnu-gcc src/prob.c -o deploy/EchoFrag
aarch64-linux-gnu-strip deploy/EchoFrag
```

# deploy
```
cd deploy
docker-compose up
```

# exploit
```
python3 exploit/exploit.py
```
