#include "global.hpp"

#include "protolua.hpp"
#include "protologic/lua_protologic.hpp"
#include "protologic/lua_globals.hpp"
#include "utils.hpp"


lua_State* protolua_init() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);
	return state;
}

// Setup protolua API.
// This is anything that requires to be in the protologic host envrionment.
// BUT, can be setup from wizer.
void protolua_setup_api_pre(lua_State* state) {
	// protologic
	luaopen_protologic(state);
	lua_setglobal(state, "protologic");
}

// Setup protolua API.
// This is anything that requires to be in the protologic host envrionment.
// AND, must be done inside the protologic host envrionment.
void protolua_setup_api_post(lua_State* state) {
	lua_protologic_set_globals(state);
}
