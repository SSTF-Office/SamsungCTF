#include <stdio.h>

#include "luqwest.h"
#include "ui.h"
#include "base64.h"

#define DEBUG

#ifdef DEBUG
void dump_stack(lua_State* L) {
    const char *s;
    size_t l;
    int top = lua_gettop(L);
    printf("Stack %d:\n", top);

    for (int i = 1; i <= top; i++) {
        switch (lua_type(L, i)) {
            case LUA_TSTRING: {
                s = lua_tostring(L, i);
                l = strlen(s);


                printf("\t\"");
                for (int j = 0; j < l; ++j) {
                    if (s[j] == '\n') printf("\\n");
                    else printf("%c", s[j]);
                }
                printf("\"\n");
            }
                break;
            case LUA_TBOOLEAN:
                printf("\t%s\n", lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("\t%g\n", lua_tonumber(L, i));
                break;
            case LUA_TNIL:
                printf("\tnil\n");
                break;
            case LUA_TTABLE:
                printf("\ttable: %p\n", lua_topointer(L,i));
                break;
            case LUA_TFUNCTION:
                printf("\tfunction: %p\n", lua_topointer(L,i));
                break;
            default:
                printf("\t%p\n", lua_topointer(L,i));
                break;
        }
    }
}
#endif

#define WINDOW_W 60
#define LUA_MAXINPUT 512
#define PROMT "» "
#define CSI_CLEAR "\033[2J"
#define CSI_CURSOR_POS(x, y) ("\033["#y";"#x"H")

#define PRINT_LINE_TOP() printf(CSI_CLEAR); \
    printf(CSI_CURSOR_POS(1,1));            \
    ornament("༺∾∾", 3, NULL, 0, "∾∾༻\n", 4, "┄", 1, WINDOW_W);

#define PRINT_LINE_MID() ornament("  ∾", 3, "⩫", 1, "┄∾  \n", 5, "┄", 1, WINDOW_W);

static Scene scene;
static bool IS_RUNNING = false;

int promt() {
    char s[LUA_MAXINPUT];

    printf(PROMT);
    fflush(stdout);

    if (fgets(s, LUA_MAXINPUT, stdin) != NULL) {
        int x = -1;
        for (int i = 0; s[i] != '\0'; ++i) {
            if (x == -1) {
                if (s[i] >= '0' && s[i] <= '9') {
                    x = s[i] - '0';
                }
            } else {
                if (s[i] >= '0' && s[i] <= '9') {
                    x = x * 10 + (s[i] - '0');
                } else break;
            }
        }

        if (x != -1) return x;

        for (int i = 0; s[i] != '\0'; ++i) {
            if (s[i] >= 'A' && s[i] <= 'Z') return s[i];
            if (s[i] >= 'a' && s[i] <= 'z') return s[i] - 32;
        }
    }

    return 0;
}

int start(lua_State *L) {
    luaL_checktype(L,1,LUA_TTABLE);

    IS_RUNNING = true;

    loop:
    while (IS_RUNNING) {
        dump_stack(L);

        lua_pushpointer(L, &scene);
        load(L);

        dump_stack(L);

        PRINT_LINE_TOP();
        printf("  ❝");
        size_t l = strlen(scene.text);
        for (int i = 0; i < l; ++i) {
            printf("%c", scene.text[i]);
            if ((i+1) % (WINDOW_W - 8) == 0) {
                printf("\n   ");
            }
        }
        printf("❞\n");
        PRINT_LINE_MID();

        for (int i = 0; i < scene.n; ++i) {
            char *s = circlize(i + 1);
            if (s != NULL) {
                printf(" %s : ", s);
            } else {
                printf(" %d: ", i + 1);
            }

            l = strlen(scene.choices[i]);
            for (int j = 0; j < l; ++j) {
                printf("%c", scene.choices[i][j]);
                if ((j+1) % (WINDOW_W - 10) == 0) {
                    printf("\n 　  ");
                }
            }
            printf("\n");
        }

        int selected;

        do {
            selected = promt();

            switch (selected) {
                case 0: continue;
                case 'Q': IS_RUNNING = false; goto loop;
            }
        } while (!(selected > 0 && selected <= scene.n));

        lua_pop(L, 1);
        go(L, selected);
    }

    if (scene.choices != NULL) {
        free(scene.choices);
        scene.choices = NULL;
    }

    return 0;
}

int welcome() {
    PRINT_LINE_TOP();
    printf("  ❝%s❞\n", "Welcome to LuQwest text adventures player!");
    PRINT_LINE_MID();
    printf(" %s : %s\n", boxify('N'), "New game");
    printf(" %s : %s\n", boxify('L'), "Load game (devs only!)");
    printf(" %s : %s\n", boxify('Q'), "Quit (why want you to do so?..)");

    do {
        int ch = promt();

        switch (ch) {
            case 'N':
            case 'L':
            case 'Q':
                return ch;
        }
    } while (true);
}

void new_game(lua_State *L, const char *filename) {
    if (luaL_dofile(L, filename)) {
        printf("⚠ ERROR: %s\n", lua_tostring(L, -1));
    }
}

void read_game_script(lua_State *L) {
    char s[LUA_MAXINPUT];
    bool started = false;

    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    do {
        printf(PROMT);
        fflush(stdout);

        if (fgets(s, LUA_MAXINPUT, stdin) != NULL) {
            if (started) {
                if (strcmp(s, "-----END GAME-----\n") == 0) break;
                luaL_addstring(&buffer, s);
            } else if (strcmp(s, "-----BEGIN GAME-----\n") == 0) {
                started = true;
            }
        } else break;
    } while (true);

    luaL_pushresult(&buffer);
}

int load_game(lua_State *L) {
    PRINT_LINE_TOP();
    printf("Enter game script:\n");

    read_game_script(L);
    const char *buffer = lua_tostring(L, -1);
    lua_pop(L, 1);

    if (base64(L, buffer)) {
        printf("⚠ ERROR: %s\n", "Bad script game encoding");
        return EXIT_FAILURE;
    }

    const char *script = lua_tostring(L, -1);
    lua_pop(L, 1);

    if (luaL_loadbuffer(L, script, strlen(script), NULL) || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        printf("⚠ ERROR: %s\n", lua_tostring(L, -1));
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("⚠ ERROR: %s\n", "No default game script");
        return EXIT_FAILURE;
    }

    lua_State *L = luaL_newstate();

//    luaL_openlibs(L);

    luaL_requiref(L, LUA_GNAME, luaopen_base, 1);
    luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
    lua_pop(L, 2);

    /* GC in generational mode */
    lua_gc(L, LUA_GCGEN, 0, 0);

    lua_register(L, "start", start);
    /* Overwrite builtin functions */
    lua_register(L, "load", load);

    lua_pushnil(L);
    lua_setglobal(L, "loadfile");

    lua_pushnil(L);
    lua_setglobal(L, "dofile");

    switch (welcome()) {
        case 'Q': break;
        case 'N': new_game(L, argv[1]); break;
        case 'L': load_game(L); break;
    }

    lua_close(L);

    return EXIT_SUCCESS;
}
