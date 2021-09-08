#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "menu.h"
#include "utils.h"

const char pictures[3][0x200] = {
    "  _                           _  \n"
    " / `._                     _.' \\ \n"
    "( @ : `.                 .' : @ )\n"
    " \\  `.  `.  ._     _.  .'  .'  / \n"
    "  \\;' `.  `.  \\   /  .'  .' `;/  \n"
    "   \\`.  `.  \\  \\_/  /  .'  .'/   \n"
    "    ) :-._`. \\ (:) / .'_.-: (    \n"
    "    (`.....,`.\\/:\\/.',.....')    \n"
    "     >------._|:::|_.------<     \n"
    "    / .'._>_.-|:::|-._<_.'. \\    \n"
    "    |o _.-'_.-^|:|^-._`-._ o|    \n"
    "    |`'   ;_.-'|:|`-._;   `'|    \n"
    "    \".o_.-' ;.\"|:|\".; `-._o.\"    \n"
    "      \".__.\"   \\:/   \".__.\"      \n"
    "                ^                \n",
    "          /x\\       /x\\                \n"
    "         /v\\x\\     /v\\/\\               \n"
    "         \\><\\x\\   /></x/               \n"
    "          \\><\\x\\ /></x/                \n"
    "  __ __  __\\><\\x/></x/___              \n"
    " /##_##\\/       \\</x/    \\__________   \n"
    "|###|###|  \\         \\    __________\\  \n"
    " \\##|##/ \\__\\____\\____\\__/          \\\\ \n"
    "   |_|   |  |  | |  | |              \\|\n"
    "   \\*/   \\  |  | |  | /              / \n"
    "           /    /                      \n",
    "  ((((c,               ,7))))  \n"
    " (((((((              )))))))) \n"
    "  (((((((            ))))))))  \n"
    "   ((((((@@@@@@@@@@@))))))))   \n"
    "    @@@@@@@@@@@@@@@@)))))))    \n"
    " @@@@@@@@@@@@@@@@@@))))))@@@@  \n"
    "@@/,:::,\\/,:::,\\@@@@@@@@@@@@@@ \n"
    "@@|:::::||:::::|@@@@@@@@@@@@@@@\n"
    "@@\\':::'/\\':::'/@@@@@@@@@@@@@@ \n"
    " @@@@@@@@@@@@@@@@@@@@@@@@@@@   \n"
    "   @@@@@@@@@@@@@@@@@@@@@@\\     \n"
    "      /    \\        (     \\    \n"
    "     (      )        \\     \\   \n"
    "      \\    /          \\        \n"
};

void catch_bug()
{
    unsigned int random;

    random = (unsigned int)rand() & 0b11;

    if(bug_count >= LEN_BUGS)
    {
        puts("Your bag is full!");
        return;
    }

    printf("Finding the bug");
    for(int i=0; i<3; i++)
    {
        printf(".");
        sleep(1);
    }
    puts("");

    if(random == 0b11)
    {
        puts("There is no bug =(");
        return;
    }
    puts("You have caught a bug!");
    puts("");

    printf(pictures[random]);
    puts("");

    puts("Name the bug");
    printf(">> ");
    read_string(bugs[bug_count].name, 4);
    bugs[bug_count].picture_id = random;
    bug_count++;
}

void inspect_bug()
{
    if(!bug_count)
    {
        puts("You did not find any bugs!");
        return;
    }

    for(int i = 0; i < bug_count; i++)
    {
        puts("=========================");
        printf(bugs[i].name);
        puts("");
        printf(pictures[bugs[i].picture_id]);
        puts("");
    }
    puts("=========================");
}

void submit_report()
{
    puts("Submit a report about your work");
    puts("Report title");
    global_report.cursor += read_string(global_report.cursor, 0x40);
    puts("Report subtitle");
    global_report.cursor += read_string(global_report.cursor, 0x80);

    for(int i = 0; i < bug_count; i++)
    {
        memcpy(global_report.cursor, bugs[i].name, 8);
        global_report.cursor += 8;
        strcpy(global_report.cursor, pictures[bugs[i].picture_id]);
        global_report.cursor += strlen(pictures[bugs[i].picture_id]);
    }
    puts("Report body");
    global_report.cursor += read_string(global_report.cursor, 0x100);
    puts("Report tag");
    global_report.cursor += read_string(global_report.cursor, 0x8);
    puts("Report password");
    read_string(global_report.password_cursor, 0x8);
}
