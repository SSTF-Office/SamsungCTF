#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "menu.h"

void init()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    bug_count = 0;
    global_report.cursor = global_report.report_buf;
    global_report.password_cursor = global_report.password;
    srand(time(NULL));
    alarm(30);
}

void print_logo()
{
    puts(" ██████╗ █████╗ ███████╗ ██████╗██╗  ██╗");
    puts("██╔════╝██╔══██╗╚═██╔══╝██╔════╝██║  ██║");
    puts("██║     ███████║  ██║   ██║     ███████║");
    puts("██║     ██╔══██║  ██║   ██║     ██╔══██║");
    puts("╚██████╗██║  ██║  ██║   ╚██████╗██║  ██║");
    puts(" ╚═════╝╚═╝  ╚═╝  ╚═╝    ╚═════╝╚═╝  ╚═╝");
    puts("███████╗██╗  ██╗███████╗");
    puts("╚═██╔══╝██║  ██║██╔════╝");
    puts("  ██║   ███████║███████╗");
    puts("  ██║   ██╔══██║██╔════╝");
    puts("  ██║   ██║  ██║███████╗");
    puts("  ╚═╝   ╚═╝  ╚═╝╚══════╝");
    puts("██████╗ ██╗  ██╗ █████╗ ");
    puts("██╔══██╗██║  ██║██╔═══╝ ");
    puts("██████╔╝██║  ██║██║ ███╗");
    puts("██╔══██╗██║  ██║██║  ██║");
    puts("██████╔╝╚█████╔╝╚█████╔╝");
    puts("╚═════╝  ╚════╝  ╚════╝");
    puts("");
}
