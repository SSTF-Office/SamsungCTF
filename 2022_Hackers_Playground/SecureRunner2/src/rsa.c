#include <stdio.h>
#include <string.h>
#include "rsa.h"

void initRSA(RSA* rsa)
{
	mpz_init2(rsa->p, RSA_BITLEN);
	mpz_init2(rsa->q, RSA_BITLEN);
	mpz_init2(rsa->n, RSA_BITLEN);
	mpz_init2(rsa->e, RSA_BITLEN);
	mpz_init2(rsa->d, RSA_BITLEN);
	mpz_init2(rsa->dp, RSA_BITLEN);
	mpz_init2(rsa->dq, RSA_BITLEN);
	mpz_init2(rsa->a, RSA_BITLEN);
	mpz_init2(rsa->b, RSA_BITLEN);
	mpz_init2(rsa->cs, RSA_BITLEN);
}

void deleteRSA(RSA* rsa)
{
	mpz_clear(rsa->p);
	mpz_clear(rsa->q);
	mpz_clear(rsa->n);
	mpz_clear(rsa->e);
	mpz_clear(rsa->d);
	mpz_clear(rsa->dp);
	mpz_clear(rsa->dq);
	mpz_clear(rsa->a);
	mpz_clear(rsa->b);
	mpz_clear(rsa->cs);
}

int generateKey(RSA* rsa)
{
	FILE* fp;
	char randbuf[RSA_BITLEN / 8 / 2];
	mpz_t temp;

	fp = fopen("/dev/urandom", "rb");
	if (fp == NULL) return -1;

	//generate p
	fread(randbuf, 1, RSA_BITLEN / 8 / 2, fp);
	randbuf[0] |= 0x80;
	mpz_import(rsa->p, RSA_BITLEN / 8 / 2, 1, 1, 1, 0, randbuf);
	mpz_nextprime(rsa->p, rsa->p);

	//generate q
	fread(randbuf, 1, RSA_BITLEN / 8 / 2, fp);
	randbuf[0] |= 0x80;
	mpz_import(rsa->q, RSA_BITLEN / 8 / 2, 1, 1, 1, 0, randbuf);
	mpz_nextprime(rsa->q, rsa->q);

	//generate e
	fread(randbuf, 1, RSA_BITLEN / 8 / 2, fp);
	randbuf[0] |= 0x80;
	mpz_import(rsa->e, RSA_BITLEN / 8 / 2, 1, 1, 1, 0, randbuf);
	mpz_nextprime(rsa->e, rsa->e);

	fclose(fp);
	
	//set n
	mpz_mul(rsa->n, rsa->p, rsa->q);	//n = p * q

	//calc d
	mpz_init(temp);
	mpz_sub_ui(rsa->a, rsa->p, 1);	//a = p - 1
	mpz_sub_ui(rsa->b, rsa->q, 1);	//b = q - 1
	mpz_mul(temp, rsa->a, rsa->b);	//temp = (p - 1) * (q - 1)
	mpz_invert(rsa->d, rsa->e, temp);
	mpz_clear(temp);

	//calc dp, dq
	mpz_mod(rsa->dp, rsa->d, rsa->a);
	mpz_mod(rsa->dq, rsa->d, rsa->b);

	//calc a
	mpz_invert(rsa->a, rsa->q, rsa->p);	//a = qinv mod p
	mpz_mul(rsa->a, rsa->a, rsa->q);	//a =  q * (qinv mod p)

	//calc b
	mpz_invert(rsa->b, rsa->p, rsa->q);	//b = pinv mod q
	mpz_mul(rsa->b, rsa->b, rsa->p);	//b =  p * (pinv mod q)

	//set checksum = xor(p, q, dp, dq)
	mpz_xor(rsa->cs, rsa->p, rsa->q);
	mpz_xor(rsa->cs, rsa->cs, rsa->dp);
	mpz_xor(rsa->cs, rsa->cs, rsa->dq);

	return 0;
}

void printRSA(RSA* rsa)
{
#ifdef _DEBUG_
	printf("p = ");
	mpz_out_str(stdout, 10, rsa->p);
	printf(" at %p", rsa->p->_mp_d);
	printf("\nq = ");
	mpz_out_str(stdout, 10, rsa->q);
	printf(" at %p", rsa->q->_mp_d);
	printf("\n");
#endif
	printf("n = ");
	mpz_out_str(stdout, 10, rsa->n);
	printf("\ne = ");
	mpz_out_str(stdout, 10, rsa->e);
	printf("\n");
#ifdef _DEBUG_
	printf("d = ");
	mpz_out_str(stdout, 10, rsa->d);
	printf("\na = ");
	mpz_out_str(stdout, 10, rsa->a);
	printf("\nb = ");
	mpz_out_str(stdout, 10, rsa->b);
	printf("\n");
#endif
}

int generateSign(unsigned char* buf, RSA *rsa)
{
	int r = 0;
	mpz_t m, sp, sq;

	//alloc mpz variables
	mpz_init(m);
	mpz_init(sp);
	mpz_init(sq);

	//set m
	mpz_import(m, strlen(buf), 1, 1, 1, 0, buf);

	//verify checksum
	mpz_xor(sp, rsa->p, rsa->q);
	mpz_xor(sp, sp, rsa->dp);
	mpz_xor(sp, sp, rsa->dq);

	if (mpz_cmp(sp, rsa->cs) != 0)
	{
		printf("error. No hack!");
		goto EXIT;
	}

	//calc sp and sq
	mpz_powm(sp, m, rsa->dp, rsa->p);
	mpz_powm(sq, m, rsa->dq, rsa->q);

	//calc CRT = a * sp + b * sq
	mpz_mul(sp, rsa->a, sp);
	mpz_mul(sq, rsa->b, sq);
	mpz_add(sp, sp, sq);
	mpz_mod(sp, sp, rsa->n);

	printf("sign = ");
	mpz_out_str(stdout, 10, sp);

EXIT:
	//clear mpz variables
	mpz_clear(m);
	mpz_clear(sp);
	mpz_clear(sq);

	return 0;
}

int verifySign(char* cmd, char* sign, RSA *rsa)
{
	mpz_t m, s, n;
	int r;

	//alloc mpz variables
	mpz_init(m);
	mpz_init(s);
	mpz_init(n);

	//set msg and signature
	mpz_import(m, strlen(cmd), 1, 1, 1, 0, cmd);
	mpz_set_str(s, sign, 10);

	//verify
	mpz_mul(n, rsa->p, rsa->q);
	mpz_powm(s, s, rsa->e, n);
	r = mpz_cmp(m, s) == 0;

	//clear mpz variables
	mpz_clear(m);
	mpz_clear(s);
	mpz_clear(n);

	return r;
}

void describeRSAKeyGen(RSA* rsa)
{
	mpz_t n;
	mpz_init(n);
	mpz_mul(n, rsa->p, rsa->q);

	puts(" 2048-bit RSA key generation sequence:");
	puts("  1) Select 1024 bit prime p, it's secret.");
	puts("  2) Select 1024 bit prime q, it's secret, too.");
	printf("  3) Calculate n as p times q, it's public. In this case, it's  ");
	mpz_out_str(stdout, 10, n);
	puts("\n  4) Calculate lcm(p-1, q-1)");
	printf("  5) Select e such that 1 < e < lcm(p-1, q-1), it's public. In this case, it's ");
	mpz_out_str(stdout, 10, rsa->e);
	puts("\n  6) Calculate d as d = pow(e, -1, lcm(p-1, q-1)), it's secret.");
	puts(" Now you have private key pair, (n, d), and public key pair, (n, e).");
}
