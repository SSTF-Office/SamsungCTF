#include <stdlib.h>
#include <stdio.h>

#include "menu.h"

void dummy0(){}

void init()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    head = 0;
}

void dummy1(){}

void greeting()
{
    puts("------------------------------------------------");
    puts("-                                              -");
    puts("-     Welcome to Automated Binary Compiler     -");
    puts("-    This binary is automatically compiled!    -");
    puts("-                                              -");
    puts("------------------------------------------------");
    puts("");
}

void dummy2(){}
