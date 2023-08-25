## 2 Outs in the Ninth Inning

### Description

You're at the bottom of the 9th with 2 outs.

Can you hit a home run at the last chance?

Download: [9end2outs.zip](deploy/download/9end2outs.zip)

Server: `nc 9end2outs.sstf.site 1337`

### Deploy
```
docker-compose up --build -d
```

### Challenge Setup
dlsym으로 libc를 열고, 임의의 함수 명을 입력받아 그 주소를 보여줌
특정 주소를 입력받아 그리로 점프하고 exit()

#### Exploit
libc 함수 두어개 정도 주소 확인해서 libc 버전 파악 및 libc 파일, libc addr 입수

one gadget offset 계산해서 점프, 쉘 획득
