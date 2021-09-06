#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>


struct Custom_header {
    char magic[4];
    unsigned int size;
    unsigned char hash[SHA256_DIGEST_LENGTH];
};


int getFileSize(char *fileName);
void getFileHash(char *fileName, unsigned char *hash);
int checkIntegrity(char *fileName, char* hash);
void init_header(char *fileName, struct Custom_header* custom_header);
