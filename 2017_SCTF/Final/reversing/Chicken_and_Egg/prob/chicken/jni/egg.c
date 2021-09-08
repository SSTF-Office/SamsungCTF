#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "aes.h"

#define LOAD_DEX_BLOB x5b3b5ed31e11d5136adaad99364586c0c91dface
#define LOOKUP_API xcba78f7093c6d5c2ef3767d822dbd2a1af7a453a
#define DECODE_STR(buf, size) do { \
    for (int j=0; j<size; j++) \
        buf[j] ^= (j+1); \
    } while(0);


typedef void (*api_t)(const uint32_t* args, jvalue* pResult);

typedef struct {
    void* clazz;
    uint32_t lock;
    uint32_t length;
    uint64_t content[1];
} ArrayObject;

void* LOAD_DEX_BLOB(JNIEnv* env, jobject assetManager)
{
    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
char buf_0[8];
buf_0[0] = 100;
buf_0[1] = 101;
buf_0[2] = 100;
buf_0[3] = 125;
buf_0[4] = 106;
buf_0[5] = 106;
buf_0[6] = 108;
buf_0[7] = 0;
DECODE_STR(buf_0, 7);
    AAsset* asset = AAssetManager_open(manager, buf_0, AASSET_MODE_UNKNOWN);
    if (asset == NULL)
        return NULL;

    int ct_size = AAsset_getLength(asset);

    uint8_t* ct = malloc(ct_size);
    AAsset_read(asset, ct, ct_size);
    AAsset_close(asset);

char buf_1[17];
buf_1[0] = 96;
buf_1[1] = 117;
buf_1[2] = 102;
buf_1[3] = 119;
buf_1[4] = 106;
buf_1[5] = 107;
buf_1[6] = 98;
buf_1[7] = 107;
buf_1[8] = 96;
buf_1[9] = 122;
buf_1[10] = 99;
buf_1[11] = 105;
buf_1[12] = 127;
buf_1[13] = 101;
buf_1[14] = 106;
buf_1[15] = 105;
buf_1[16] = 0;
DECODE_STR(buf_1, 16);
    char* key = buf_1;
char buf_2[17];
buf_2[0] = 105;
buf_2[1] = 99;
buf_2[2] = 109;
buf_2[3] = 96;
buf_2[4] = 118;
buf_2[5] = 105;
buf_2[6] = 106;
buf_2[7] = 109;
buf_2[8] = 96;
buf_2[9] = 100;
buf_2[10] = 98;
buf_2[11] = 120;
buf_2[12] = 123;
buf_2[13] = 107;
buf_2[14] = 108;
buf_2[15] = 100;
buf_2[16] = 0;
DECODE_STR(buf_2, 16);
    char* iv = buf_2;
    uint8_t* pt = malloc(ct_size);
    AES128_CBC_decrypt_buffer(pt, ct, ct_size, (uint8_t*)key, (uint8_t*)iv);
    int pt_size = ct_size - pt[ct_size - 1];

    int arr_size = pt_size + sizeof(ArrayObject);
    ArrayObject* arr = (ArrayObject*) malloc(arr_size);
    memset(arr, 0, arr_size);

    arr->length = pt_size;
    memcpy(arr->content, pt, pt_size);

    free(ct);
    free(pt);
    return arr;
}

inline api_t LOOKUP_API(JNINativeMethod* dexFile, const char* name, const char* sig)
{
    api_t result = NULL;
    int i = 0;
    while (dexFile[i].name != NULL)
    {
        if (!strcmp(name, dexFile[i].name) &&
            !strcmp(sig, dexFile[i].signature))
        {
            result = (api_t)dexFile[i].fnPtr;
            break;
        }
        i++;
    }
    return result;
}

jint
Java_com_example_chicken_Crypt_crackEgg(JNIEnv* env, jobject thiz,
                                        jobject assetManager)
{

char buf_3[10];
buf_3[0] = 109;
buf_3[1] = 107;
buf_3[2] = 97;
buf_3[3] = 96;
buf_3[4] = 115;
buf_3[5] = 107;
buf_3[6] = 41;
buf_3[7] = 123;
buf_3[8] = 102;
buf_3[9] = 0;
DECODE_STR(buf_3, 9);
    void* libdvm = dlopen(buf_3, RTLD_LAZY);
    if (libdvm == NULL)
        return 0;

char buf_4[26];
buf_4[0] = 101;
buf_4[1] = 116;
buf_4[2] = 110;
buf_4[3] = 91;
buf_4[4] = 97;
buf_4[5] = 103;
buf_4[6] = 107;
buf_4[7] = 126;
buf_4[8] = 96;
buf_4[9] = 97;
buf_4[10] = 84;
buf_4[11] = 127;
buf_4[12] = 116;
buf_4[13] = 125;
buf_4[14] = 123;
buf_4[15] = 117;
buf_4[16] = 124;
buf_4[17] = 77;
buf_4[18] = 87;
buf_4[19] = 113;
buf_4[20] = 109;
buf_4[21] = 80;
buf_4[22] = 126;
buf_4[23] = 116;
buf_4[24] = 124;
buf_4[25] = 0;
DECODE_STR(buf_4, 25);
    JNINativeMethod* dexFile = (JNINativeMethod*)dlsym(libdvm, buf_4);
    if (dexFile == NULL)
        return 0;

char buf_5[12];
buf_5[0] = 110;
buf_5[1] = 114;
buf_5[2] = 102;
buf_5[3] = 106;
buf_5[4] = 65;
buf_5[5] = 99;
buf_5[6] = 127;
buf_5[7] = 78;
buf_5[8] = 96;
buf_5[9] = 102;
buf_5[10] = 110;
buf_5[11] = 0;
DECODE_STR(buf_5, 11);
char buf_6[6];
buf_6[0] = 41;
buf_6[1] = 89;
buf_6[2] = 65;
buf_6[3] = 45;
buf_6[4] = 76;
buf_6[5] = 0;
DECODE_STR(buf_6, 5);
    api_t openDexFile = LOOKUP_API(dexFile, buf_5, buf_6);
    if (openDexFile == NULL)
        return 0;

    void* blob = LOAD_DEX_BLOB(env, assetManager);
    if (blob == NULL)
        return 0;

    uint32_t args[] = { (uint32_t)blob };
    jvalue res;
    openDexFile(args, &res);
    return res.i;
}