#pragma once


#define INIT_FILE "init.lua"


lua_State* protolua_init();

int protolua_package_searcher(lua_State* state);
void protolua_setup_files(lua_State* state);

void protolua_setup_api_pre(lua_State* state);
void protolua_setup_api_post(lua_State* state);
