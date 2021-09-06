#include "./file.h"

void init_header(char *fileName, struct Custom_header* custom_header){
    int count=0,len=0,i=0;
    char buf[1024];
    strcpy(custom_header->magic,"SCTF");
    custom_header->size = getFileSize(fileName);
    getFileHash(fileName,custom_header->hash);
}

void getFileHash(char *fileName,unsigned char *hash){
    int count=0,len=0;
    char buf[1024]={0,};
    int size = getFileSize(fileName);
    int fd = open(fileName,O_RDONLY,S_IRUSR|S_IWUSR);
    
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    while(1){
        memset(buf,0,1024);
        if ( count >= size )
            break;
        else if ( count + 1024 > size )
            len = size % 1024;
        else
            len = 1024;

        read(fd,buf,len);
        SHA256_Update(&ctx,buf,len);
        count += len;
    }
    SHA256_Final(hash,&ctx);
        close(fd);
}

int getFileSize(char *fileName){
    int size=0;
    int fd = open(fileName,O_RDONLY,S_IRUSR|S_IWUSR);
    if (fd == -1)
        return -1;
    size = lseek(fd,0,SEEK_END);
    close(fd);
    return size;
}


int checkIntegrity(char *fileName,char *hash){
    unsigned char m[SHA256_DIGEST_LENGTH];
    getFileHash(fileName,m);
    int i = 0;
    if(!memcmp(m,hash,SHA256_DIGEST_LENGTH))
        return 1;
    else
        return 0;
}
