#include "global.hpp"

#include "protolua.hpp"
#include "protologic/lua_protologic.hpp"
#include "protologic/lua_globals.hpp"
#include "utils.hpp"


lua_State* protolua_init() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);

	// lua `coroutine` library doesn't work due to our `longjmp` hack.
	lua_pushnil(state);
	lua_setglobal(state, "coroutine");

	lua_pushnil(state);
	lua_setglobal(state, "pcall");

	lua_pushnil(state);
	lua_setglobal(state, "xpcall");

	return state;
}

// Setup protolua API.
// This is anything that requires to be in the protologic host envrionment.
void protolua_setup_api(lua_State* state) {
	// protologic
	luaopen_protologic(state);
	lua_setglobal(state, "protologic");

	lua_protologic_set_globals(state);
}
