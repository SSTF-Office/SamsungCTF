#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "auto.h"



void init_auto()
{
    setvbuf(stdin,0,_IONBF,0);
    setvbuf(stdout, 0, _IONBF, 0);
    alarm(40);
}

void print_help()
{
    printf("build your own DFA and test it!\n");
}

int print_menu()
{
    puts("    1. add node");
    puts("    2. del node");
    puts("    3. make step");
    puts("    4. set start node");
    puts("    5. mark final node");
    puts("    6. view status");
    puts("    7. test input");
    puts("    8. exit");
    printf(">> ");
    return 1;
}

int main()
{
    char linebuf[0x100];
    init_auto();
    print_help();
    while(print_menu())
    {
        fgets_eof(linebuf, 0x100);
        switch(atoi(linebuf))
        {
            case 1:
                add_node();
                break;
            case 2:
                del_node();
                break;
            case 3:
                make_step();
                break;
            case 4:
                set_start();
                break;
            case 5:
                mark_final();
                break;
            case 6:
                view_status();
                break;
            case 7:
                test_input();
                break;
            case 8:
                exit(0);
        }
    }
}
