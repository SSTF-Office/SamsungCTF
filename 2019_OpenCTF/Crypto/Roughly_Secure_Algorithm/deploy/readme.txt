[Info]
Title:Roughly Secure Algorithm
Description:Crypto is hard? well...
Attach:Roughly_Secure_Algorithm.zip

[Deploy]
 Not necessary

[Solving Strategy]
 1. we can calculate the modulus by using 2 plaintext-ciphertext pairs
   pt0 ^ e mod N = ct0, so pt0 ^ e - ct0 = k1 * N
   pt1 ^ e mod N = ct1, so pt1 ^ e - ct1 = k2 * N

   gcd(pt0 ^ e - ct0, pt1 ^ e - ct1) == gcd(k1 * N, k2 * N) == kN, where k is very small value with very high probability
   so we can calculate N with little brute-force
 2. p and q are very similar(p ~ q), so, N = pq ~ p*p
   we can find p by little brute-force from sqr(N)
 3. from p, we can calculate calculate q, phi(N), and d
 4. with d, we can decrypt RSA and get the flag
