// gcc -o dvchat dvchat.c -lncurses -lpthread
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <seccomp.h>
#include <sys/prctl.h>

#define RECV_H 15
#define RECV_W 40
#define RECV_Y 1
#define RECV_X (2+10)

#define SEND_H 5
#define SEND_W 55
#define SEND_Y 16
#define SEND_X (2+10)

#define LIST_H 15
#define LIST_W 14
#define LIST_Y 1
#define LIST_X (43+10)

typedef struct __record{
    unsigned int no;
    char* ip;
    char* port;
    char* id;
    struct __record* next;
}record, *precord;

typedef struct _MSGQ{
    struct _MSGQ* next;
    char* msg;
}MSGQ, *PMSGQ;

void trim(char* s);
void draw_server_window();
PMSGQ g_head;
char* g_id;
char* g_pw;
int lock=0;
char* g_log;
char* prompt;
WINDOW* g_server_win;
WINDOW* g_list_win;
int g_bot_n;
char g_buf[1024];

void sandbox(){
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
    if (ctx == NULL) {
        printf("seccomp error\n");
        exit(0);
    }

    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sigaction), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigaction), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(nanosleep), 0);

    if (seccomp_load(ctx) < 0){
        seccomp_release(ctx);
        printf("seccomp error\n");
        exit(0);
    }
    seccomp_release(ctx);
}

void queue_msg(char* msg){
    PMSGQ tmp = (PMSGQ)malloc(sizeof(MSGQ));
    memset(tmp, 0, sizeof(MSGQ));
    tmp->msg = malloc(512);
    memset(tmp->msg, 0, 512);
    snprintf(tmp->msg, 512, "%s", msg);
    tmp->next = g_head;
    g_head = tmp;
}

void draw_list_window(WINDOW* win){
    int i=0;
    if(!lock){
        wclear(win);

        lock++;
        for(i=0; i<g_bot_n; i++){
            mvwprintw(win, 1+i, 1, "bot");
            box(win, 0, 0);
            wrefresh(win);
        }
        lock--;
    }
}

void draw_server_window(WINDOW* win){
    int i=0;
    PMSGQ tmp=0;
    if(!lock){
        wclear(win);
        lock++;
        tmp = g_head;
        while(tmp != 0){
            mvwprintw(win, 1+i, 1, "%s", tmp->msg);
            i++;
            tmp = tmp->next;
            if(i > RECV_H-2) break;
        }
        box(win, 0, 0);
        wrefresh(win);
        lock--;
    }
}

void trim(char* s){
    while(s[strlen(s)-1]=='\n' || s[strlen(s)-1]==' '){
        s[strlen(s)-1]=0;
    }
}

char* cursewords[] = {"beach", "duck", "sheet", 0};
char* nicerwords[] = {"puppy", "love", "goood", 0};
void* server_thread(void* param){
    sandbox();
    int i=0;
    PMSGQ tmp;
    char* p;
    char* p2;
    char buf[100];
    WINDOW* cli_win = (WINDOW*)param;
    g_server_win = newwin(RECV_H, RECV_W, RECV_Y, RECV_X);
    draw_list_window(g_server_win);
    while(1){
        usleep(1000*100);
        tmp = g_head;
        while(tmp != 0){
            i=0;
            // check for curse words every secs.
            for(p = cursewords[i]; p; p = cursewords[++i]){
                if(p2 = strstr(tmp->msg, p)){
                    strncpy(p2, nicerwords[i], strlen(nicerwords[i]));
                    sprintf(buf, "bad word '%s' is replaced to nice word '%s'\n", p, nicerwords[i]);
                    strcat(g_log, buf);
                }
            }
            tmp = tmp->next;
        }
    }
}

void* list_thread(void* param){
    sandbox();
    WINDOW* cli_win = (WINDOW*)param;
    WINDOW* win = newwin(LIST_H, LIST_W, LIST_Y, LIST_X);
    g_list_win = win;
    draw_list_window(win);
    while(1){
        sleep(10);
    }
}

char* get_input(WINDOW* win){
    char c;
    char buf[64];
    memset(buf, 0, 64);
    strncpy(buf, prompt, strlen(prompt));
    char* res = malloc(64);
    int i=strlen(prompt);

    while(c = mvgetch(SEND_Y+1, SEND_X+1+i)){
        if(c=='\n'){
            buf[i] = 0;
            break;
        }
        if(c==127){         // ncurses back space ascii number: 127
            if(i==strlen(prompt)) continue;
            i--;
            if(!lock){
                lock++;
                box(win, 0, 0);
                mvwprintw(win, 1, 1+i, " ");
                wrefresh(win);
                lock--;
            }
            continue;
        }

        if(i<38){
            buf[i++] = c;
        }
        if(!lock){
            lock++;
            box(win, 0, 0);
            mvwprintw(win, 1, 1, "%s", buf);
            wrefresh(win);
            lock--;
        }
    }

    strcpy(res, buf);
    return res;
}

void loop(WINDOW* win){
    char buf[16];
    char* cmd2;
    while(1){
        wclear(win);
        if(g_server_win) draw_server_window(g_server_win);
        if(g_list_win) draw_list_window(g_list_win);
        if(!lock){
            lock++;
            box(win, 0, 0);
            mvwprintw(win, 1, 1, "%s", prompt);
            wrefresh(win);  // only refresh the window
            lock--;
        }

        cmd2 = get_input(win);
        if(!strcmp(cmd2+strlen(prompt), "/quit")){
            endwin();
            printf("ncurses closed\n");
            free(g_id);
            free(g_pw);
            return;
        }
        if(!strcmp(cmd2+strlen(prompt), "/addbot")){
            g_bot_n++;
            continue;
        }
        queue_msg(cmd2);
    }
    buf[0]=0;
}

void main(int argc, char* argv[]){
    char buf[16];

    putenv("TERM=xterm");
    pthread_t th1=0;
    pthread_t th2=0;

    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin, 0, _IOLBF, 0);

    // init stuffs...
    memset(g_buf, 0, 1024); 
    prompt = &g_buf[1000];
    g_log = g_buf;
    strcpy(prompt, "> ");
    g_bot_n=1;
    g_head = 0;
    srand(time(0));
    g_id = malloc(32);
    g_pw = malloc(32);
    printf("enter your ID: ");
    scanf("%32s", g_id);
    printf("enter your PW: ");
    scanf("%32s", g_pw);
    trim(g_id);
    trim(g_pw);
    
    // bootup NCURSES!
    initscr();
    noecho();

    /* setup client window */
    WINDOW* win = newwin(SEND_H, SEND_W, SEND_Y, SEND_X);
    refresh();  // refresh entire screen

    pthread_create( &th1, 0, server_thread, (void*)win);
    pthread_detach( th1 );

    pthread_create( &th2, 0, list_thread, (void*)win);
    pthread_detach( th2 );

    // draw a box at (y,x)
    box(win, 0, 0);

    usleep(30000);
    loop(win);
    buf[0]=0;
}


