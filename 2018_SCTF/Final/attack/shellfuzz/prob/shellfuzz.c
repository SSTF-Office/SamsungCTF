#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>

char input[100];

int main(int argc, char *argv[]){
            
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin, 0, _IOLBF, 0);

    int fd = open("/dev/urandom", O_RDONLY);
    if(fd==-1){
        printf("error. tell admin\n");
        return 0;
    }

    printf("welcome to system call fuzzing world!\n");
    printf("fuzzing is so cool. it generates strange results!\n");
    printf("for example, maybe it can give you kernel crash!\n");

    // we fuzz syscall input parameters.
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    if(read(fd, &eax, 4)!=4){ printf("error eax!\n"); return -1; }

    eax = eax % 300;
    // smart fuzzing!(?!) we avoid some system calls...
    while(eax == SYS_ptrace || eax == SYS_execve || eax == SYS_open || eax == SYS_fork || eax == SYS_vfork){
        if(read(fd, &eax, 4)!=4){ printf("error eax!\n"); return -1; }
        eax = eax % 300;
    }

    // prepare random page with random contents
    void* randpage = mmap((void*)0x10000, 0x1000, 7, MAP_ANONYMOUS | MAP_FIXED | MAP_PRIVATE, 0, 0);
    if(read(fd, randpage, 0x1000)!=0x1000){ printf("error making random memory!\n"); return -1; }

    // set up random syscall parameters
    if(read(fd, &ebx, 4)!=4){ printf("error ebx!\n"); return -1; }
    if(read(fd, &ecx, 4)!=4){ printf("error ecx!\n"); return -1; }
    if(read(fd, &edx, 4)!=4){ printf("error edx!\n"); return -1; }
    if(read(fd, &esi, 4)!=4){ printf("error esi!\n"); return -1; }
    if(read(fd, &edi, 4)!=4){ printf("error edi!\n"); return -1; }

    // smart fuzzing!(?) we select integer and pointer
    // in case pointer, we make VALID pointers, that points random contents
    if(ebx%2){
        ebx = 0x10000 + (ebx % 0x1000);
    }
    if(ecx%2){
        ecx = 0x10000 + (ecx % 0x1000);
    }
    if(edx%2){
        edx = 0x10000 + (edx % 0x1000);
    }
    if(esi%2){
        esi = 0x10000 + (esi % 0x1000);
    }
    if(edi%2){
        edi = 0x10000 + (edi % 0x1000);
    }
    
    printf("syscall parameters all set!\n");
    printf("eax:%d ebx:%x ecx:%x edx:%x esi:%x edi:%x\n", eax, ebx, ecx, edx, esi, edi);
    printf("password?\n");
    gets(input);
    if(strncmp(input, "daehee", 6)){
        printf("wrong password!\n");
        exit(0);
    }

    printf("fuzzing start! good luck!\n");
    syscall(eax, ebx, ecx, edx, esi, edi);

    printf("you should not see this message if you have crash!\n");
    return 0;
}


