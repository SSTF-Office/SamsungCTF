#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "auto.h"


struct DFA_node_iter* head = NULL;
struct DFA_node* start_node = NULL;

char* fgets_eof(char* buf, size_t len)
{
    char* retval = fgets(buf, len, stdin);
    if(feof(stdin))
    {
        printf("EOF reached\n");
        exit(0);
    }
    if(strlen(buf) >= 1 && buf[strlen(buf)-1]=='\n')
        buf[strlen(buf)-1] = '\x00';
    return retval;
}


struct DFA_node* find_DFA_by_name(char* buf)
{
    struct DFA_node_iter* iter = head;
    for(; iter; iter = iter->next)
    {
        if(!strcmp(iter->elem->name,buf))
            return iter->elem;
    }
    return NULL;
}

void del_DFA_by_name(char* buf)
{
    struct DFA_node_iter init_dummy;
    struct DFA_node_iter* iter = &init_dummy;
    iter->next = head;
    for(; iter && iter->next ; iter = iter->next)
    {
        struct DFA_rule init_dummy2;
        struct DFA_rule* iter2 = &init_dummy2;
        iter2->next = iter->next->elem->rule;
        for(; iter2 && iter2->next; iter2 = iter2->next)
        {
            if(!strcmp(iter2->next->node->name, buf))
            {
                struct DFA_rule* t2 = iter2->next;
                if(iter2->next == iter->next->elem->rule)
                {
                    iter->next->elem->rule = iter2->next->next;
                }
                iter2->next = iter2->next->next;
                free(t2);
            }
        }
    }
    iter = &init_dummy;
    for(; iter && iter->next ; iter = iter->next)
    {
        if(!strcmp(iter->next->elem->name,buf))
        {
            if(iter->next == head)
            {
                head = iter->next->next;
            }
            if(iter->next->elem == start_node)
            {
                start_node = NULL;
            }

            struct DFA_node_iter* target = iter->next;
            iter->next = iter->next->next;
            free(target->elem);
            free(target);
        }
    }

    return;
}


void add_node_to_iter(struct DFA_node* target)
{
    struct DFA_node_iter* con = (struct DFA_node_iter*)malloc(sizeof(struct DFA_node_iter));
    con->elem = target;
    con->next = head;
    head = con;
    return;
}

struct DFA_node* find_rule(struct DFA_node* target, char sym)
{
    struct DFA_rule* iter = target->rule;
    for(; iter; iter = iter->next)
    {
        if(iter->val == sym)
            return iter->node;
    }
    return NULL;

}

void add_node()
{
    char linebuf[0x100];
    unsigned int namelen = 0;
    printf("length of name: ");
    fgets_eof(linebuf, 0x100);
    namelen = atoi(linebuf);
    if(namelen + 1 > 0x100)
    {
        puts("invalid length");
        return;
    }
    printf("name of node: ");
    namelen = read(0,linebuf,namelen);
    if(namelen > 1 && linebuf[namelen-1] == '\n')
        linebuf[namelen-1] = '\x00';
    else
        linebuf[namelen] = '\x00';
    if(find_DFA_by_name(linebuf))
    {
        puts("name exists");
        return;
    }
    struct DFA_node* retval = (struct DFA_node*)malloc(sizeof(struct DFA_node));
    retval->is_final = 0;
    retval->rule = NULL;
    retval->name = strdup( linebuf );
    add_node_to_iter(retval);
    puts("add successful\n");
    return;
}

void del_node()
{
    char linebuf[0x100];
    printf("name of node: ");
    fgets_eof(linebuf, 0x100);
    if(!find_DFA_by_name(linebuf))
    {
        puts("can't find node");
        return;
    }
    del_DFA_by_name(linebuf);
    puts("del successful\n");
    return;
}

void make_step()
{
    char linebuf[0x100];
    printf("start node: ");
    fgets_eof(linebuf, 0x100);
    struct DFA_node* snode = find_DFA_by_name(linebuf);
    if(!snode)
    {
        puts("can't find node");
        return;
    }
    printf("symbol: ");
    fgets_eof(linebuf,0x100);
    if(strlen(linebuf) != 1)
    {
        puts("character must be 1byte");
        return;
    }
    if(find_rule(snode, *linebuf))
    {
        puts("alreay defined for other node");
        return;
    }
    char symv = *linebuf;
    printf("end node: ");
    fgets_eof(linebuf, 0x100);
    struct DFA_node* fnode = find_DFA_by_name(linebuf);
    if(!fnode)
    {
        puts("can't find node");
        return;
    }
    struct DFA_rule* srule = (struct DFA_rule*)malloc(sizeof (struct DFA_rule));
    srule->val = symv;
    srule->node = fnode;
    srule->next = snode->rule;
    snode->rule = srule;
    puts("add successful");
    return;

}

void set_start()
{
    char linebuf[0x100];
    printf("name of node: ");
    fgets_eof(linebuf, 0x100);
    struct DFA_node* node = find_DFA_by_name(linebuf);
    if(!node)
    {
        puts("can't find node");
        return;
    }
    start_node = node;
    puts("set successful");
    return;
}

void mark_final()
{
    char linebuf[0x100];
    printf("name of node: ");
    fgets_eof(linebuf, 0x100);
    struct DFA_node* node = find_DFA_by_name(linebuf);
    if(!node)
    {
        puts("can't find node");
        return;
    }
    node->is_final = 1;
    puts("mark successful");
    return;
}
void view_rule(struct DFA_node* x)
{
    struct DFA_rule* rule = x->rule;
    for(;rule; rule = rule->next)
    {
        printf(" %s - %c -> %s\n", x->name, rule->val, rule->node->name);
    }
}
void view_status()
{
    struct DFA_node_iter* iter = head;
    for(; iter; iter = iter->next)
    {
        printf("[node %s]", iter->elem->name);
        if(iter->elem->is_final)
            printf(" - final");
        printf("\n");
        view_rule(iter->elem);
    }
    return;

}

void test_input()
{
    char linebuf[0x1000];
    int loc = 0;
    struct DFA_node* cur = start_node;
    if(!start_node)
    {
        puts("have to set start node");
        return;
    }
    printf("input >> ");
    fgets_eof(linebuf, 0x1000);
    for(; linebuf[loc]; loc++)
    {
        cur = find_rule(cur, linebuf[loc]);
        if(!cur)
        {
            puts("REJECT");
            return;
        }
    }
    if(cur->is_final)
    {
        puts("ACCEPT");
        return;
    }
    else
    {
        puts("REJECT");
        return;
    }

}


