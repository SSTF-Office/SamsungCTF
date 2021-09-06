#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

#define CBC 1

#define AES128_CBC_decrypt_buffer x03c0a4d7caef0216bd70ce0f624666db6d3fab55
void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

#endif //_AES_H_
