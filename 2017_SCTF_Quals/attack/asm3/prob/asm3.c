// gcc -o asm3 asm3.c -m32 -pie
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#define LEN 30
#define STUBLEN 22
char stub[] = "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\x31\xff\x31\xf6\x31\xed\x31\xe4\x65\xa3\x10\x00\x00\x00";

// block x86 system call gadgets
int sandbox(char* shellcode){
    // block syscall via "int 0x80"
    if(strstr(shellcode, "\xcd\x80")) return 1;
    // block syscall via "sysenter"
    if(strstr(shellcode, "\x0f\x34")) return 1;
    return 0;
}

int main(int argc, char* argv[]){

    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin, 0, _IOLBF, 0);

    int fd = open("/dev/urandom", O_RDONLY);
    if(fd<0){
        printf("can't open /dev/urandom\n");
        exit(0);
    }
    int seed;
    read(fd, &seed, 4);
    srand(seed);

    unsigned int addr = rand()*0xdeadbeef;
    addr &= 0xfffff000;
    char* sh = (char*)mmap(addr, 0x1000, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_FIXED|MAP_PRIVATE, 0, 0);
    memcpy(sh, stub, STUBLEN);
    printf("give me your shellcode:");
    read(0, sh+STUBLEN, LEN);

    int i;
    for(i=0; i<LEN; i++){
        if(sh[STUBLEN+i] == 0){
            printf("remove null bytes in your input\n");
            exit(0);
        }
    }

    if( sandbox(sh+STUBLEN) ){
        printf("caught by filter!\n");
        exit(0);
    }
    
    mprotect(addr, 0x1000, 5);
    alarm(30);
    printf("wait 10 sec, before evaluating your shellcode...\n");
    sleep(10);
    printf("let me see\n");
    asm("jmp *%0" :: "r"(sh));
    return 0;
}


