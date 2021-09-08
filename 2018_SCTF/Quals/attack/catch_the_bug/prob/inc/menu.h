#ifndef _MENU_H_
#define _MENU_H_

#define LEN_BUGS 3

typedef struct _bug
{
    char name[0x8];
    unsigned int picture_id;
} Bug;

typedef struct _report
{
    char report_buf[0x700];
    char password[0x8];
    char *cursor;
    char *password_cursor;
} Report;

int bug_count;
Bug bugs[LEN_BUGS];
Report global_report;

void catch_bug();
void inspect_bug();
void submit_report();

#endif
