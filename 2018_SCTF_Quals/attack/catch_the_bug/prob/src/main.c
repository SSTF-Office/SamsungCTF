#include <stdio.h>

#include "init.h"
#include "utils.h"
#include "menu.h"

int main(void)
{
    int c;
    init();
    print_logo();
    while(1)
    {
        print_menu();
        c = read_int();
        switch(c)
        {
            case 1:
                catch_bug();
                break;
            case 2:
                inspect_bug();
                break;
            case 3:
                submit_report();
            case 0:
                goto EXIT;
            default:
                puts("Illegal menu");
                break;
        }
    }
EXIT:
    return 0;
}
