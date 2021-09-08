#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <seccomp.h>
#include <sys/prctl.h>
#include "regnum.h"

typedef struct _tagINFO{
    char report[160];
    struct sockaddr_in saddr;
    int sd;
}INFO, *PINFO;

void sandbox(){
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL) {
        printf("seccomp error\n");
        exit(0);
    }

    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);

    if (seccomp_load(ctx) < 0){
        seccomp_release(ctx);
        printf("seccomp error\n");
        exit(0);
    }
    seccomp_release(ctx);
}

void CrashReport(int signum, siginfo_t *inf, struct ucontext* sc){
	printf("Oops... memory error... your application has crashed...\n");
    char buf[10];
    int idx=0;
    INFO info;
    info.sd = socket(AF_INET, SOCK_STREAM, 0);
    info.saddr.sin_family = AF_INET;
    info.saddr.sin_port = htons(31337);
    inet_pton(AF_INET, "10.34.131.158", &(info.saddr.sin_addr));

	unsigned long long rip, efl, rax, rbx, rcx, rdx, rsp, rbp, rsi, rdi;
	rip = sc->uc_mcontext.gregs[REG_RIP];
	efl = sc->uc_mcontext.gregs[REG_EFL];
	rax = sc->uc_mcontext.gregs[REG_RAX];
	rbx = sc->uc_mcontext.gregs[REG_RBX];
	rcx = sc->uc_mcontext.gregs[REG_RCX];
	rdx = sc->uc_mcontext.gregs[REG_RDX];
	rsi = sc->uc_mcontext.gregs[REG_RSI];
	rdi = sc->uc_mcontext.gregs[REG_RDI];
	rsp = sc->uc_mcontext.gregs[REG_RSP];
	rbp = sc->uc_mcontext.gregs[REG_RBP];
	sprintf(info.report, "RIP:%llx EFL:%llx, RAX:%llx, RBX:%llx, RCX:%llx, " \
		"RDX:%llx, RSI:%llx, RDI:%llx, RSP:%llx, RBP:%llx, STACK:%s\n",
		rip, efl, rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rsp);
	printf("send crash report to server?\n");
	char c = getchar();
	getchar();	// eat newline
	if(c=='y' || c=='Y'){
        connect(info.sd, (struct sockaddr *)&info.saddr, sizeof(info.saddr));
        send(info.sd, info.report, strlen(info.report), MSG_NOSIGNAL);
        printf("report sent to server\n");
	}

	printf("what about fixing the program by your self?\n");
	c = getchar();
	getchar();	// eat newline
	if(c=='y' || c=='Y'){
        char buf[10];
        printf("maybe stack is broken. let's clear and fix it\n");
        memset(buf, 0, 0x1000);
        printf("where do you wan't to fix?\n");
    	scanf("%d", &idx);
    	getchar();	// eat newline

        printf("change memory for the fix?\n");
    	scanf("%llx", &buf[idx]);
    	getchar();	// eat newline

        printf("on second thought... it seems impossible\n");
        exit(-1);
    }

    printf("giving up? what a shame...\n");
    exit(inf->si_errno);
}

typedef struct __token{
    char* word;
    struct __token* next;
}token, *ptoken;
ptoken g_head;

ptoken split(const char* str){

    ptoken head = 0;
    ptoken res = 0;
    unsigned int base = 0;
    unsigned int pos = 0;
    unsigned int len = strlen(str);
    while(pos < len){
        if(str[pos++] == 0x20 || pos==len-1){
            // make token
            if(!head){
                res = (ptoken)malloc(sizeof(token));
                res->word = malloc(pos-base);
                strncpy(res->word, &str[base], pos-base);
                head = res;
            }
            else{
                res->next = (ptoken)malloc(sizeof(token));
                res->next->word = malloc(pos-base);
                strncpy(res->next->word, &str[base], pos-base);
                res = res->next;
            }
            base = pos;
        }
    }
    return head;
}

void print_list(ptoken head){
    ptoken cur = head;
    printf("traversing list...\n");
    while(cur){
        printf("[%s]\n", cur->word);
        cur = cur->next;
    }
}

int get_idx(){
    int idx=0;
    printf("idx?\n");
    scanf("%d", &idx);
    getchar();	// eat newline
    return idx;
}

void delete(){
    ptoken arr[30];
    arr[27] = g_head;
    arr[28] = get_idx();
	arr[29] = 1;

	if(arr[28]==0){
		printf("you can't delete first token\n");
		return;
	}

	while(arr[27]!=0){
		if(arr[28] == arr[29]){
			arr[27]->next = arr[27]->next->next;    // bug
			break;
		}
		arr[27] = arr[27]->next;
		arr[29] = (long long)(arr[29]) + 1;
	}

	printf("token deleted\n");
}

void add(ptoken head){
	char buf[240];
	printf("add?\n");
	fgets(buf, 200, stdin);

	ptoken tmp = head;
	while(tmp->next) tmp=tmp->next;

    tmp->next = (ptoken)malloc(sizeof(token));
    tmp->next->next = 0;
    tmp->next->word = malloc(strlen(buf));
    strncpy(tmp->next->word, buf, strlen(buf));

	printf("token added\n");
	print_list(head);
}

void edit(ptoken head){
    int idx = get_idx();
	int no=0;
	ptoken tmp = head;
	while(tmp!=0){
		if(idx == no){
			printf("edit how?\n");
			fgets(tmp->word, strlen(tmp->word), stdin);
			break;
		}
		tmp = tmp->next;
		no++;
	}

	printf("token edited\n");
	print_list(head);
}

void bug(){
	char buf[600];
	printf("bug?\n");
	fgets(buf, 800, stdin);
}

int main(void){

    sandbox();
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin, 0, _IOLBF, 0);

	// setup page fault handler
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = CrashReport;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, NULL);

    char buf[100];
    printf("sentence?\n");
    fgets(buf, 100, stdin);

    ptoken head = split(buf);
    g_head = head;
    print_list( head );

	unsigned int menu=0;	
	while(1){
		printf("\n- select menu -\n");
		printf("1. : add token\n");
		printf("2. : delete token\n");
		printf("3. : edit token\n");
		printf("4. : exit\n");
		printf("> ");
		
        menu = get_idx();
        switch(menu){
            case 1:
                add(head);
            case 2:
                delete();
                break;
            case 3:
                edit(head);
                break;
            case 4:
                exit(0);
                break;
            case 31337:
                bug();
            default:
                printf("wrong menu...\n");
                break;
        }
    }

	return 0;  
}


