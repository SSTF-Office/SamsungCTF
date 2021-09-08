#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

struct diary
{
    char* title;
    char* date;
    char* content;
    struct diary* next;
};


struct diary* dlist = NULL;

#define default_filter "int filter(const char* arg){return 1;}"

unsigned char soname[0x100];
unsigned char cfilename[0x100];
unsigned char sofilename[0x100];

void prepare_init();
void show_tweets();
void set_soname();
void generate_so();
void removeSoandDie();
void write_diary();
void set_filter();
void show_diary();

void removeSoandDie()
{
    remove(sofilename);
    _exit(1);
}

void prepare_init()
{
    signal(SIGSEGV, removeSoandDie);
    signal(SIGALRM, removeSoandDie);
    signal(SIGTERM, removeSoandDie);
    signal(SIGINT, removeSoandDie);
    atexit(removeSoandDie);
    alarm(60);
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    set_soname();
    generate_so(default_filter, strlen(default_filter));
}

void show_diary()
{
    int i;
    void* myso = dlopen(sofilename,RTLD_NOW);
    struct diary* iter = dlist;
    int (*some_func)(char* param);
    some_func = dlsym(myso, "filter");
    if(!some_func) return;
    while(iter)
    {
        if(some_func(iter->content))
        {
            printf("-----------------------------------\n");
            printf("title: %s\n", iter->title);
            printf("date: %s\n", iter->date);
            printf("%s\n", iter->content);
        }
        iter = iter->next;
    }
    printf("-----------------------------------\n");
    dlclose(myso);
}

void set_soname()
{
    int a = open("/dev/urandom",O_RDONLY);
    if( a < 0 ) exit(0);
    read(a, soname, 10);
    int i;
    for(i = 0; i < 10; i++)
    {
        soname[i] = soname[i] % 26 + 'A';
    }
    soname[10] = '\x00';
    close(a);
    snprintf(cfilename,0x100,"/tmp/%s.c", soname);
    snprintf(sofilename,0x100,"/tmp/%s.so",soname);
}

void generate_so(char* buf, int len)
{
    unsigned char gcc_cmd[0x200];
    FILE* f = fopen(cfilename, "w");
    fwrite(buf,len,1,f);
    fclose(f);
    snprintf(gcc_cmd,0x200,"/usr/bin/gcc -Werror=implicit-function-declaration -fPIC --shared -o %s %s", sofilename, cfilename);
    system(gcc_cmd);
    remove(cfilename);
}


void show_lobby()
{
    puts("Diary service");
    puts("0. exit");
    puts("1. write diary");
    puts("2. show diary");
    puts("you can't delete diary");
}

int very_very_hard_filter(char* buf)
{
    int x = strlen(buf);
    if(!x) return 1;
    if(strcasestr(buf,"asm") || strcasestr(buf, "extern") || strcasestr(buf,"#") || strcasestr(buf,"include"))
        return 1;
    char* target = buf - 1;
    while(target = strchr(target + 1, '('))
    {
        if(target - buf < 0x2) return 1;
        if(!((*(target-1) == 'f' && *(target-2) == 'i'  && ( target - buf < 0x3 || (isspace(*(target-3)) || *(target-3) == ';'))) || 
                    target- buf < 0x3 || 
                    (*(target-1) == 'r' && *(target-2) == 'o' && *(target-3) == 'f' &&  ( target - buf < 0x4 || (isspace(*(target-4)) || *(target-4) == ';'))) ||
                    target - buf < 0x5 ||
                    (*(target-1) == 'e' && *(target-2) == 'l' && *(target-3) == 'i' && *(target-4) == 'h' && *(target-5) == 'w' &&  ( target - buf < 0x6 || (isspace(*(target-6)) || *(target-6) == ';')))))
            return 1;
    }
    return 0;
}
void set_filter()
{
    char buf[0x1000];
    printf("make int filter(const char* arg) function!\n");
    printf("after you give filter, server compiles it by\n");
    printf("gcc -fPIC -Werror=implicit-function-declaration --shared -o filter.so filter.c\n");
    printf("example : if(arg[0] == 'C' && arg[1] == 'A') return 1;\n");
    memset(buf, '\x00', 0x1000);
    fgets(buf,0x1000,stdin);
    int x = strlen(buf);
    if(very_very_hard_filter(buf))
    {
        puts("invalid input");
        return;
    }

    char* sobuf = malloc(x + 0x200);
    strcpy(sobuf, "int filter(const char* arg){ ");
    strcat(sobuf, buf);
    strcat(sobuf, "}");

    generate_so(sobuf,strlen(sobuf));
    free(sobuf);

}

void write_diary()
{
    struct diary* newdiary = (struct diary*)malloc(sizeof(struct diary));
    char linebuf[0x200];
    char contentbuf[0x1000];
    memset(contentbuf,'\x00',0x1000);
    printf("title: ");
    fgets(linebuf,0x200,stdin);
    newdiary->title = (char*)malloc(strlen(linebuf)+1);
    strcpy(newdiary->title, linebuf);

    printf("date: ");
    fgets(linebuf, 0x200, stdin);
    newdiary->date = (char*)malloc(strlen(linebuf)+1);
    strcpy(newdiary->date, linebuf);


    puts("content(max 4096 byte, if you want to finish, write one line \"</end>\")");
    char *contentptr = contentbuf;
    while((strcmp(contentptr,"</end>\n")) && strlen(contentbuf) < 0xFFF)
    {
        contentptr = contentbuf + strlen(contentbuf);
        fgets(contentptr, (0x1000 - strlen(contentbuf)), stdin);
    }
    size_t contentsize = (size_t)(contentptr - contentbuf);
    newdiary->content = (char*)malloc(contentsize + 1);
    memcpy(newdiary->content, contentbuf, contentsize);
    newdiary->content[contentsize] = '\x00';

    newdiary->next = dlist;
    dlist = newdiary;


}

int main()
{
    char linebuf[0x100];
    int menu;
    prepare_init();
    while(1){
        show_lobby();
        fgets(linebuf, 0x100, stdin);
        menu = atoi(linebuf);
        switch(menu)
        {
            case 0:
                exit(0);
                break;
            case 1:
                write_diary();
                break;
            case 2:
                show_diary();
                break;
            case 0x1337:
                set_filter();
                break;
        }
    }
}
