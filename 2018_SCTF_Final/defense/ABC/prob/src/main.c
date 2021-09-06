#include <stdio.h>

#include "init.h"
#include "utils.h"
#include "menu.h"

int main(void)
{
    int c;
    init();
    greeting();
    while(1)
    {
        print_menu();
        c = read_int();
        switch(c)
        {
            case 1:
                new_memo();
                break;
            case 2:
                view_memo();
                break;
            case 3:
                modify_memo();
                break;
            case 4:
                delete_memo();
                break;
            case 5:
                goto EXIT;
            default:
                puts("Illegal menu");
                break;
        }
    }
EXIT:
    return 0;
}
