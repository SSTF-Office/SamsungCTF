#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int read_int()
{
    char buf[0x10];

    buf[read(0, buf, 0xf)] = '\0';
    return atoi(buf);
}

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

void print_menu()
{
    puts("------------------------");
    puts("-     Program Menu     -");
    puts("------------------------");
    puts("- 1. Catch a bug       -");
    puts("- 2. Inspect the bug   -");
    puts("- 3. Submit a repport  -");
    puts("- 0. Exit              -");
    puts("------------------------");
    printf(">> ");
}
