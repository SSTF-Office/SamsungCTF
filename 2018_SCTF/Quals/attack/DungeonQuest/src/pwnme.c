#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct skillMacro {
    struct skillMacro *nxt;
    struct skillMacro *prv;
    int size;
    char *word;
    int (*dmgFunc) (int);
};

struct _g{
    int64_t demon_hp;
    int64_t hp;
    struct skillMacro *head;
} g;

/* XXX: overwrite head with unsorted bin attck and then trigger it*/
int skill_fireball(int x) {
    return 100;
}
int skill_iceball(int x) {
    return 1000 + x/2000;
}
int skill_meteo(int x) {
    return x/100;
}

int init_macro() {
    int (*dmgFuncs[3]) (int) = {skill_fireball, skill_iceball, skill_meteo};
    for (int i = 0; i < 3; i++) {
        struct skillMacro *x = (struct skillMacro *)malloc(sizeof(struct skillMacro));
        x->nxt = g.head;
        x->prv = NULL;
        x->dmgFunc = dmgFuncs[i];
        x->word = NULL;
        g.head = x;
    }
}

struct skillMacro *find_skill(int i) {
    struct skillMacro *n = g.head;
    for(int j = 0; j < 3 && j != i; j++){
        n = n->nxt;
    }
    return n;
}

int readline(char *buf, int length) {
    char ch;
    int i;
    for(i = 0; i < length; i++) {
        if(read(0, &ch, 1) == 1) {
            if(ch == '\n') {
                buf[i] = 0;
                return i;
            }
            buf[i] = ch;
        } else {
            buf[i] = 0;
            return i;
        }
    }
    buf[i-1] = 0;
}
int add_macro() {
    void (*dmgFunc) (int) = NULL;
    char buf[0x20] = {0,};
    int i;
    printf("Which skill> ");
    readline(buf, 0x20 - 1);

    if(!strcmp(buf, "fireball")) {
        i = 0;
    } else if(!strcmp(buf, "iceball")) {
        i = 1;
    } else if(!strcmp(buf, "meteo")) {
        i = 2;
    } else {
        printf("No such skill\n");
        return 0;
    }
    char buf2[0x400];
    memset(buf2, 0, 0x400);
    printf("Which word to say?\n");
    struct skillMacro *m = find_skill(i);
    if(m->word) {
        int sz = readline(buf2, m->size);
        memcpy(m->word, buf2, sz);
    } else {
        int sz = readline(buf2, 0x400-1);
        sz = sz + 1 < 0x100 ? 0x100 - 1 : sz;
        m->size = sz;
        m->word = calloc(1, sz + 1);
        memcpy(m->word, buf2, sz);
    }
}

int remove_skill() {
    char buf[0x20] = {0,};
    int i;

    printf("Which skill> ");
    readline(buf, 0x20 - 1);

    if(!strcmp(buf, "fireball")) {
        i = 0;
    } else if(!strcmp(buf, "iceball")) {
        i = 1;
    } else if(!strcmp(buf, "meteo")) {
        i = 2;
    } else {
        printf("No such skill\n");
        return 0;
    }
    struct skillMacro *m = find_skill(i);
    free(m->word);
    printf("macro for skill %s successfully deleted.\n", buf);
}

int readint() {
    char buf[10] = {0,};
    int x = read(0, buf, 9);
    return atoi(buf);
}

void skill_demon_blackout() {
    printf("Demon cast blackout!\n");
    close(1);
    close(2);
}

void skill_demon_doom() {
    printf("Demon cast doom!\n");
    g.hp = 0;
}

void do_skill(int x) {
    struct skillMacro *m = find_skill(x);
    g.demon_hp -= m->dmgFunc(g.demon_hp);
    if(m->word) {
        printf("You shout %s!", m->word);
    }
}

void fight() {
    printf("You encountered demon!\n");
    skill_demon_blackout();
    int x;
    while (1) {
        printf("1) skill fireball\n");
        printf("2) skill iceball\n");
        printf("3) skill meteo\n");
        printf("Which skill you wanna cast>");
        x = readint();
        do_skill(x-1);
        skill_demon_doom();
        if (g.hp <= 0) {
            printf("you are dead bye!\n");
            exit(0);
        }
    }
}

int skillmanage() {
    while (1) {
        printf("1) add macro\n");
        printf("2) delete macro\n");
        printf("3) go back\n");
        printf("> ");
        int x = readint();
        switch(x) {
            case 1:
                add_macro();
                break;
            case 2:
                remove_skill();
                break;
            default:
                return 0;
        }
    }
}

int main() {
    g.hp = 0x100;
    g.demon_hp = 0x800000;
    g.head = NULL;
    init_macro();
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    alarm(10);
    printf("Welcome to dungeon run!\n");
    do {
        printf("1) Go front\n");
        printf("2) Managing skill\n");
        printf("3) Exit\n");
        printf("Choice> ");
        switch(readint()) {
            case 1:
                fight();
                break;
            case 2:
                skillmanage();
                break;
            default:
                printf("bye\n");
                exit(0);
                break;
        }
    } while(1);
}
