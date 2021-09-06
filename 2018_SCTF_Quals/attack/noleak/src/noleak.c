// gcc -o noleak noleak.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define UINT unsigned int

int menu1(){
    printf("give me bytes\n");
    char* hbuf = malloc(200);
    fgets(hbuf, 150, stdin);

    printf("info leak with uninitialized bytes?\n");
    fwrite(hbuf, 4, 1, stdout);
    free(hbuf); 
    return 0;
}

int menu2(){
    char a[1]={0};
    int buf[99];
    int hash=0;
    int i;
    for(i=0; i<99; i++){
        buf[i] = 0x31337;
    }
    for(i=0; i<100; i++){
        hash += buf[i];
    }
    printf("info leak with oob read? %x\n", hash);
    return 0;
}

int menu3(){
    char buf[100];
    int* idx = &buf[92];
    memset(buf, 0, 100);

    // prevent leak
    printf("you may start stack BOF but...\n");
    printf("no memory leak from now!\n");
    close(1);
    close(2);

    char* hbuf = malloc(100);
    fgets(hbuf, 116, stdin);
    while(*idx < 200){
        buf[*idx] = hbuf[*idx];
        (*idx)++;
    }
    stdin = 0xdeadbeef;
    return 0;
}

unsigned int seed;
int main(){
    char buf[24];
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin, 0, _IOLBF, 0);

    FILE* fp = fopen("/dev/urandom", "r");
    if(fp==0){
        printf("cannot open /dev/urandom\n");
        exit(0);
    }
    fread(&seed, 4, 1, fp);
    fclose(fp);

    printf("making less predictable heap\n");
    malloc(seed % 0x20000);  // increase heap entropy

    UINT menu=0;	
    while(1){
		printf("\n- select menu -\n");
		printf("1. : leak memory contents\n");
		printf("2. : leak stack canary\n");
		printf("3. : start bof\n");
		printf("4. : exit\n");
		printf("> ");
		
		scanf("%d", &menu);
		getchar();	// eat newline

        switch(menu){
            case 1:
                menu1();
            break;
            case 2:
                menu2();
            break;
            case 3:
                menu3();
            break;
            case 0xfeedbeef:
                system("echo flag");
            default:
                exit(0);
            break;
        }
    }

    printf("bye\n");
    return 0;
}


