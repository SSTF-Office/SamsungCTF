/*
 * Copyright (c) 2016, Linaro Limited
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

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "aes_impl.h"
#include <rss_ta.h>

#define UNUSED(x) (void)(x)

#define BUFLEN 512

struct cipher_t {
    unsigned char ciphertext[BUFLEN];
    unsigned char n_blocks;
    unsigned char padding[15]; // AARCH64 GCC doesn't insert padding :(
    unsigned char encKey[BUFLEN];
} cipher;

void *getKey(char *keybuf);
TEE_Result encrypt_store(void *buf, size_t len);
TEE_Result decrypt_restore(void *buf);
/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */

/* For Debugging TZ 
static int isprint(int c)
{
    return (unsigned)c-0x20 < 0x5f;
}

static int isspace(int c)
{
    return ((c == ' ') || (c == '\n') || (c == '\t'));
}


static void hexdump(const void * memory, size_t bytes) {
    const unsigned char * p, * q;
    int i;

    p = memory;
    while (bytes) {
        q = p;
        printf("%p: ", (void *) p);
        for (i = 0; i < 16 && bytes; ++i) {
            printf("%02X ", *p);
            ++p;
            --bytes;
        }
        bytes += i;
        while (i < 16) {
            printf("XX ");
            ++i;
        }
        printf("| ");
        p = q;
        for (i = 0; i < 16 && bytes; ++i) {
            printf("%c", isprint(*p) && !isspace(*p) ? *p : ' ');
            ++p;
            --bytes;
        }
        while (i < 16) {
            printf(" ");
            ++i;
        }
        printf(" |\n");
    }
    return;
}
*/

static size_t _strlen(const unsigned char *string)
{
    size_t len = 0;

    while (*string++)
        len++;
    return len;
}

static int _strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
}

TEE_Result TA_CreateEntryPoint(void)
{

    TEE_MemFill(cipher.encKey, 0, BUFLEN);
    TEE_MemFill(cipher.ciphertext, 0, BUFLEN);
    getKey(cipher.encKey);
    //DMSG("KeyDump");
    //hexdump(cipher.encKey, 1024);
    return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
        TEE_Param __maybe_unused params[4],
        void __maybe_unused **sess_ctx)
{
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
            TEE_PARAM_TYPE_NONE,
            TEE_PARAM_TYPE_NONE,
            TEE_PARAM_TYPE_NONE);

    DMSG("has been called");

    if (param_types != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    /* Unused parameters */
    (void)&params;
    (void)&sess_ctx;

    /* If return value != TEE_SUCCESS the session will not be created. */
    return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
    (void)&sess_ctx; /* Unused parameter */
    IMSG("Goodbye!\n");
}

void* getKey(char *keybuf) {
    TEE_Result res;
    TEE_PropSetHandle h;

    res = TEE_AllocatePropertyEnumerator(&h);
    TEE_StartPropertyEnumerator(h, TEE_PROPSET_TEE_IMPLEMENTATION);

    char nbuf[BUFLEN];
    uint32_t nblen = sizeof(nbuf);
    char vbuf[BUFLEN];
    uint32_t vblen = sizeof(vbuf);

    while (true) {
        TEE_GetPropertyName(h, nbuf, &nblen);
        if(_strcmp(nbuf, "gpd.tee.trustedos.manufacturer") == 0){
            res = TEE_GetPropertyAsString(TEE_PROPSET_TEE_IMPLEMENTATION, nbuf, vbuf, &vblen);

            if (keybuf != NULL) {
                TEE_MemMove(keybuf, vbuf, vblen);
            }
            break;
        }
        res = TEE_GetNextProperty(h);
        if (res = TEE_SUCCESS) {
            if (res == TEE_ERROR_ITEM_NOT_FOUND)
                return 0;
        }

    }
}

TEE_Result encrypt_store(void *buf, size_t len) {
    char tmp[1024];
    unsigned long rk[RKLENGTH(AES_256)];
    size_t i;


    TEE_MemFill(tmp, 0, sizeof(tmp));
    TEE_MemMove(tmp, buf, len); // (Actually, I want to make 3-Level Challenges for Exploit Trustzone... But, anyway :P)

    (void)rijndaelSetupEncrypt(rk, cipher.encKey, AES_256);
    cipher.n_blocks = len / (AES_BLOCK_SIZE / 8);

    for (i = 0; i < len / (AES_BLOCK_SIZE / 8); i++) {
        rijndaelEncrypt(rk, NROUNDS(AES_256), &tmp[i * (AES_BLOCK_SIZE /8)], &cipher.ciphertext[i * (AES_BLOCK_SIZE / 8)]);
    }
    //hexdump(&cipher, 2048);
    DMSG("N Blocks: %d", cipher.n_blocks);
    return TEE_SUCCESS;

}

TEE_Result decrypt_restore(void *buf) {
    unsigned long rk[RKLENGTH(AES_256)];
    size_t i;

    uint32_t count;

    (void)rijndaelSetupDecrypt(rk, cipher.encKey, AES_256);

    unsigned char *plaintext = buf;
    for (i = 0; i < cipher.n_blocks; i++) {
        rijndaelDecrypt(rk, NROUNDS(AES_256), &cipher.ciphertext[i * (AES_BLOCK_SIZE / 8)], &plaintext[i * (AES_BLOCK_SIZE / 8)]);
    }
    return TEE_SUCCESS;
}

static TEE_Result ta_rss_store(uint32_t param_types, TEE_Param params[4]) {

    if (param_types !=
            TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
                TEE_PARAM_TYPE_NONE)) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    return encrypt_store(params[0].memref.buffer, params[0].memref.size);
}


static TEE_Result ta_rss_restore(uint32_t param_types, TEE_Param params[4]) {
    if (param_types !=
            TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
                TEE_PARAM_TYPE_NONE)) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    return decrypt_restore(params[0].memref.buffer);
}

static TEE_Result ta_rss_debug(uint32_t param_types, TEE_Param params[4]) {

    if (param_types !=
            TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
                TEE_PARAM_TYPE_NONE)) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    //hexdump(cipher.ciphertext, 1024);
    TEE_MemMove(params[0].memref.buffer, cipher.ciphertext, cipher.n_blocks * AES_BLOCK_SIZE / 8);

    return TEE_SUCCESS;
}
/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
        uint32_t cmd_id,
        uint32_t param_types, TEE_Param params[4])
{
    (void)&sess_ctx; /* Unused parameter */

    switch (cmd_id) {
        case TA_RSS_STORE:
            ta_rss_store(param_types, params);
            break;
        case TA_RSS_RESTORE:
            ta_rss_restore(param_types, params);
            break;
        case TA_RSS_DEBUG:
            ta_rss_debug(param_types, params);
            break;
        default:
            UNUSED(ta_rss_store);
            UNUSED(param_types);
            UNUSED(params);
            return TEE_ERROR_BAD_PARAMETERS;
    }
    return TEE_SUCCESS;
}
