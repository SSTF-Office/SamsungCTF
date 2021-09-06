#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void dummy8(){}

int read_int()
{
    char buf[READ_INT_BUFFER_SIZE];

    buf[read(0, buf, READ_INT_REAL_SIZE)] = '\0';
    return atoi(buf);
}

void dummy9(){}

int read_string(char *dst, int len)
{
    int n = 0;
    while(n < len)
    {
        n += read(0, dst + n, len - n);
        if((n > 0 && (dst[n - 1] == '\0' || dst[n - 1] == '\n')) || n == len)
        {
            dst[n - 1] = '\0';
            break;
        }
    }
    return n;
}

void dummy10(){}

void print_menu()
{
    puts("1. New memo");
    puts("2. View memo");
    puts("3. Modify memo");
    puts("4. Delete memo");
    puts("5. Exit");
    printf(">> ");
}
