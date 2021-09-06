# GluedWindow

## Description
Now what if you cannot get all errors? Can you break it this one?

소스 코드와 log 같이 공개

## Comment
[Fault injection in sliding window alg](http://web.eecs.umich.edu/~valeria/research/publications/DATE10RSA.pdf)

BrokenWindow와 같으나 error를 상위 64비트에 대한 공격과 하위 64비트에
대한 공격을 빼고 줌.

따라서 d의 msb 부분을 얻을 수 없고 BrokenWindow에서 쓴 공격은 d를
msb부터 알아가야하는 공격이므로 d의 msb부분을 다른 방법으로 알아내야함.

그런데 ed = k(p - 1)(q - 1) + 1 에서 d ~ kn / e, 1 <= k <= e 이므로
d의 상위 비트를 Bruteforce를 통해 맞출 수 있다.

이렇게 알아낸 d의 상위 비트는 attack log 를 이용해 같은 값이 나오는지
확인함으로서 구한 비트가 맞다는 것을 검증할 수 있고 나머지 비트들은
log로 부터 알아내면 된다.

이제 남은 것은 d의 하위 64인데, ed = kn - k(p + q) + k + 1로 부터
p, q의 상위 비트들을 알아낼 수 있다. 따라서 Coppersmith's attack으로
남은 비트를 알아내면 된다.

## How to Run
python GluedWindow.py [flag] > log.txt

flag의 길이는 적절한 길이어야함 (200 ~ 1000 bits)

## Requirements
#### python
pip install pycrypto

gmpy2 (option)
