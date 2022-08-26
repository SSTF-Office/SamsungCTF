#ifndef LUQWEST_LUQWEST_H
#define LUQWEST_LUQWEST_H

#include <stdbool.h>
#include <stdlib.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"

typedef struct {
    const char *text;
    const char **choices;
    int n;
} Scene;

void go(lua_State *L, int selected);
int load(lua_State *L);

/** @Note: Intentional bad design, do not pass C pointer to Lua and vice versa */
void lua_pushpointer(lua_State *L, void *p);

/** @Note: Intentional bad design, do not pass C pointer to Lua and vice versa */
Scene *lua_toscene(lua_State *L);

#endif //LUQWEST_LUQWEST_H
