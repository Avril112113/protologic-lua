#pragma once

#include "global.hpp"


int protolua_stacktrace(lua_State *L);

int protolua_pcall(lua_State* L, int nargs, int nret);

void protolua_dumpstack(lua_State *L);