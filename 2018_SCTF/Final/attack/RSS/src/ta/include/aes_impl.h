/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AES_IMPL_H
#define AES_IMPL_H

int rijndaelSetupEncrypt(unsigned long *rk, const unsigned char *key,
			 int keybits);

int rijndaelSetupDecrypt(unsigned long *rk, const unsigned char *key,
			 int keybits);

void rijndaelEncrypt(const unsigned long *rk, int nrounds,
		     const unsigned char plaintext[16],
		     unsigned char ciphertext[16]);

void rijndaelDecrypt(const unsigned long *rk, int nrounds,
		     const unsigned char ciphertext[16],
		     unsigned char plaintext[16]);

#define AES_BLOCK_SIZE		128

#define AES_128			128
#define AES_192			192
#define AES_256			256

#define KEYLENGTH(keybits)	((keybits)/8)
#define RKLENGTH(keybits)	((keybits)/8+28)
#define NROUNDS(keybits)	((keybits)/32+6)

#endif
