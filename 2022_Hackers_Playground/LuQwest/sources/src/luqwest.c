#include "luqwest.h"

void lua_pushpointer(lua_State *L, void *p) {
    lua_pushinteger(L, (lua_Integer) p);
}

Scene *lua_toscene(lua_State *L) {
    Scene *room = (Scene*) lua_tointeger(L, -1);
    lua_pop(L, 1);
    return room;
}

const char* get_text(lua_State *L) {
    lua_getfield(L, -1, "text");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        return NULL;
    }
    const char *text = lua_tostring(L, -1);
    lua_pop(L, 1);
    return text;
}

void call_onEnter(lua_State *L) {
    lua_getfield(L, -1, "onEnter");
    if (!lua_isnil(L, -1)) {
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
    } else {
        lua_pop(L, 1);
    }
}

void call_onSelect(lua_State *L) {
    lua_getfield(L, -1, "onSelect");
    if (!lua_isnil(L, -1)) {
        lua_pushvalue(L, -2);
        lua_pushvalue(L, -4);
        lua_call(L, 2, 0);
    } else {
        lua_pop(L, 1);
    }
}

void call_onLeave(lua_State *L) {
    lua_getfield(L, -3, "onLeave");
    if (!lua_isnil(L, -1)) {
        lua_pushvalue(L, -4);
        lua_pushvalue(L, -3);
        lua_call(L, 2, 0);
    } else {
        lua_pop(L, 1);
    }
}

int call_isEnabled(lua_State *L) {
    int isEnabled = 1;

    lua_getfield(L, -1, "isEnabled");
    if (lua_isfunction(L, -1)) {
        lua_pushvalue(L, -2);
        lua_pushvalue(L, -6);
        lua_call(L, 2, 1);

        if (lua_isboolean(L, -1)) {
            if (!lua_toboolean(L, -1)) {
                isEnabled = 0;
            }
        } else {
            isEnabled = -1;
        }
    } else if (lua_isboolean(L, -1)) {
        if (!lua_toboolean(L, -1)) {
            isEnabled = 0;
        }
    }
    lua_pop(L, 1);

    return isEnabled;
}

/**
    { id = "A", text = "A", isEnabled = true },
    { id = "B", text = "B", isEnabled = true },
    { id = "C", text = "C", isEnabled = false },
    { id = "D", text = "D", isEnabled = false },
    { id = "E", text = "E", isEnabled = true },

 Stack 5:
	table: 0x5555557977b0   = room
	table: 0x5555557978c0   = E
	table: 0x555555797900   = B
	table: 0x555555797aa0   = A
	3                       = n
 * @param L
 */
void iterate_choices(lua_State *L) {
    Scene *scene = lua_toscene(L);

    lua_Integer n = 0;
    lua_pushinteger(L, n);

    lua_getfield(L, -2, "choices");
    if (!lua_isnil(L, -1)) {
        /* first key */
        lua_pushnil(L);

        while (lua_next(L, -2) != 0) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */

            if (call_isEnabled(L)) {
                lua_pushvalue(L, -1);
                lua_insert(L, -(5+n));

                lua_pushinteger(L, ++n);
                lua_replace(L, -5);
            }

            /* removes 'value'; keeps 'key' for next iteration */
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    if (n != 0) {
        scene->n = lua_tointeger(L, -1);
        if (scene->choices != NULL) {
            free(scene->choices);
            scene->choices = NULL;
        }
        scene->choices = malloc(n * sizeof(char *));

        for (int i = 1; i <= n; ++i) {
            lua_pushvalue(L, -(i + 1));
            scene->choices[i - 1] = get_text(L);
            lua_pop(L, 1);
        }
    }
}

int load(lua_State *L) {
    Scene *scene = lua_toscene(L);

    call_onEnter(L);

    const char *text = get_text(L);
    if (text != NULL) {
        scene->text = text;
    }

    lua_pushpointer(L, scene);
    iterate_choices(L);

    /** @Warning: Intentional pointer leakage */
    lua_pushpointer(L, scene->choices);

    return 2;
}

void go(lua_State *L, int selected) {
    int n = lua_tointeger(L, -1);

    if (selected < 1 || selected > n) {
        lua_pop(L, 2);
        return;
    }

    lua_pop(L, 1);

    for (int i = 1; i <= n && i != selected; ++i) {
        lua_remove(L, -1);
    }

    for (int i = selected; i < n; ++i) {
        lua_remove(L, -2);
    }

    call_onSelect(L);

    lua_getfield(L, -1, "to");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_pushvalue(L, -2);   // go to self then
    }

    call_onLeave(L);

    lua_remove(L, -2);
    lua_remove(L, -2);
}