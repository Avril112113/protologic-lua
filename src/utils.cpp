#include "global.hpp"

#include "protolua.hpp"

#include "utils.hpp"


// From: https://github.com/sydlawrence/CorsixTH-HTML5-Port/blob/95d272e0ad27758fed7d231d8ab7f79aa4b0773f/source/CorsixTH/Src/main.cpp#L201
int protolua_stacktrace(lua_State *L) {
    // Put error msg to top of stack, then clear the stack keeping only the err msg
    lua_insert(L, 1);
    lua_settop(L, 1);

    // err = tostring(err)
    lua_getglobal(L, "tostring");
    lua_insert(L, 1);
    lua_call(L, 1, 1);

    // return debug.traceback(err, 2)
    lua_getglobal(L, "debug");
    lua_getfield(L, -1, "traceback");
    lua_remove(L, -2);  // Remove `debug` table
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);
    lua_remove(L, -2);  // Remove original error string, for some reason it wasn't consumed.

    return 1;
}

// From: https://stackoverflow.com/questions/30021904/lua-set-default-error-handler
int protolua_pcall(lua_State* L, int nargs, int nret) {
	int hpos = lua_gettop(L) - nargs;
	lua_pushcfunction(L, protolua_stacktrace);
	lua_insert(L, hpos);
	int ret = lua_pcall(L, nargs, nret, hpos);
	lua_remove(L, hpos);
	return ret;
}

// From https://stackoverflow.com/questions/59091462/from-c-how-can-i-print-the-contents-of-the-lua-stack
void protolua_dumpstack(lua_State *L) {
    debug("Lua stack dump:\n");
    int top = lua_gettop(L);
    for (int i=1; i <= top; i++) {
        debug("\t%d\t%s\t\n", i, luaL_typename(L,i));
        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                debug("\t\t%g\n", lua_tonumber(L,i));
                break;
            case LUA_TSTRING:
                debug("\t\t%s\n", lua_tostring(L,i));
                break;
            case LUA_TBOOLEAN:
                debug("\t\t%s\n", (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                debug("\t\t%s\n", "nil");
                break;
            default:
                debug("\t\t%p\n", lua_topointer(L,i));
                break;
        }
    }
}