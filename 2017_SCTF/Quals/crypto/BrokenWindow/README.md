# BrokenWindow

## Description
Attacker can make fault sign by inserting error. That error making can
be possible by using laser and flip a byte in memory. Now, can attacker
recover the private key by inserting error on an iteration in windowing
method?

소스 코드와 log 같이 공개

## Comment
[Fault injection in sliding window alg](http://web.eecs.umich.edu/~valeria/research/publications/DATE10RSA.pdf)

매 injection 마다 window size 만큼의 Bruteforce로 RSA private key d의
일부 비트를 구할 수 있음.

매 Error 마다 그 순간 known bit와 이번 guess를 dl,
unknown bit를 dr이라고 하면, 간단한 Error에 대한 식을 만들어
dl이 실제 d의 상위 비트들과 같은지 확인할 수 있다.


## How to Run
python BrokenWindow.py [flag] > log.txt

flag의 길이는 적절한 길이어야함 (200 ~ 1000 bits)

## Requirements
#### python
pip install pycrypto

gmpy2 (option)
