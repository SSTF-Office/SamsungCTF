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
    AAsset* asset = AAssetManager_open(manager, XXOBFUS("eggyolk"), AASSET_MODE_UNKNOWN);
    if (asset == NULL)
        return NULL;

    int ct_size = AAsset_getLength(asset);

    uint8_t* ct = malloc(ct_size);
    AAsset_read(asset, ct, ct_size);
    AAsset_close(asset);

    char* key = XXOBFUS("awesomecipherkey");
    char* iv = XXOBFUS("handsomeinitvect");
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

    void* libdvm = dlopen(XXOBFUS("libdvm.so"), RTLD_LAZY);
    if (libdvm == NULL)
        return 0;

    JNINativeMethod* dexFile = (JNINativeMethod*)dlsym(libdvm, XXOBFUS("dvm_dalvik_system_DexFile"));
    if (dexFile == NULL)
        return 0;

    api_t openDexFile = LOOKUP_API(dexFile, XXOBFUS("openDexFile"), XXOBFUS("([B)I"));
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
