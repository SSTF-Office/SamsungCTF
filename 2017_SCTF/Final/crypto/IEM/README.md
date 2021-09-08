# IEM

## Description
I love cryptography. How about you?

[link](#) - (logging 부분 삭제하고 소스 공개)

## Comment
Iterated Even Mansour를 활용한 sliding attack이 가능한 블록암호.

우선, Permutation을 만드는 과정이 Fiestel이므로 Involution 이 된다.

따라서 Encryption 과 Decryption이 매우 비슷.

이를 이용하면 c1, c2 = E(m1), E(m2) 이고 m1 ^ c2 = k1 이면

m2 ^ c1 = k1 이므로 이를 sliding pair의 판별식으로 사용할 수 있다.

비슷하게 k2를 구하는 판별식을 만들어 사용하면 됨.

