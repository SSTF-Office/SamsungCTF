#ifndef _MENU_H_
#define _MENU_H_

#define LEN_BUGS 3

typedef struct _memo MEMO;

struct _memo
{
    char name[0x10];
    char *buf;
    short length;
    MEMO *next;
};

MEMO *head;

void new_memo();
void view_memo();
void modify_memo();
void delete_memo();

#endif
