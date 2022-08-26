#ifndef _RSA_H_

#include <gmp.h>

#define RSA_BITLEN		2048

typedef struct RSA {
	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t e;
	mpz_t d;
	mpz_t dp;	//for CRT: dp = d mod (p - 1)
	mpz_t dq;	//for CRT: dq = d mod (q - 1)
	mpz_t a;	//for CRT: q * (qinv mod p)
	mpz_t b;	//for CRT: p * (pinv mod q)
	mpz_t cs;
} RSA;

void initRSA(RSA* rsa);
void deleteRSA(RSA* rsa);
int generateKey(RSA* rsa);
int generateSign(unsigned char* buf, RSA *rsa);
int verifySign(char* cmd, char* sign, RSA *rsa);
void printRSA(RSA* rsa);
void describeRSAKeyGen(RSA* rsa);

#endif //_RSA_H_
