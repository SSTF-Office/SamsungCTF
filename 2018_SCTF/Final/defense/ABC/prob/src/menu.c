#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "menu.h"
#include "utils.h"

const char aS1[] = "[%s]\n";
const char aS2[] = "%s";

void dummy3(){}

void new_memo()
{
    unsigned short length;
    MEMO *elem;

    elem = (MEMO*)malloc(sizeof(MEMO));
    printf("New memo name: ");
    read_string(elem->name, 0x10);

    printf("New memo length: ");
    length = (unsigned short)read_int();
    elem->length = length;
    elem->buf = (char*)malloc(length);

    printf("New memo contents: ");
    read_string(elem->buf, length);

    elem->next = head;
    head = elem;
}

void dummy4(){}

void view_memo()
{
    char buf[0x20];
    MEMO *ptr;
    printf("View memo name: ");
    read_string(buf, 0x20);

    for (ptr = head; ptr != NULL; ptr = ptr->next)
    {
        if (!strcmp(buf, ptr->name))
            break;
    }
    if (ptr)
    {
        printf(aS1, ptr->name);
        __asm__ __volatile__ ("lea %0, %%rdi;" :: "m" (aS1));
        printf(ptr->buf);
    }
}

void dummy5(){}

void modify_memo()
{
    char buf[0x20];
    unsigned short length;
    MEMO *ptr;
    printf("Modify memo name: ");
    read_string(buf, 0x20);

    for (ptr = head; ptr != NULL; ptr = ptr->next)
    {
        if (!strcmp(buf, ptr->name))
            break;
    }
    if (ptr)
    {
        printf("New memo length: ");
        length = (unsigned short)read_int();
#ifdef STAGE3
        if ((short)length > (short)ptr->length)
#else
        if (length > ptr->length)
#endif
            ptr->buf = (char*)realloc(ptr->buf, length);
        ptr->length = (unsigned short)length;

        printf("New memo contents: ");
        read_string(ptr->buf, (unsigned short)length);
    }
}

void dummy6(){}

void delete_memo()
{
    char buf[0x20];
    MEMO *ptr, *prev;
    printf("Delete memo name: ");
    read_string(buf, 0x20);

    prev = NULL;
    for (ptr = head; ptr != NULL; prev = ptr, ptr = ptr->next)
    {
        if (!strcmp(buf, ptr->name))
        {
            if (prev)
                prev->next = ptr->next;
            else
                head = ptr->next;
            free(ptr->buf);
            free(ptr);
            break;
        }
    }
}

void dummy7(){}
