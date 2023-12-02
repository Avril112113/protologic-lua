#include "global.hpp"

#include "protolua.hpp"
#include "lua_protologic.hpp"
#include "utils.hpp"


lua_State* protolua_init() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);

	luaopen_protologic(state);
	lua_setglobal(state, "protologic");

	// lua `coroutine` library doesn't work due to our `longjmp` hack.
	lua_pushnil(state);
	lua_setglobal(state, "coroutine");

	lua_pushnil(state);
	lua_setglobal(state, "pcall");

	lua_pushnil(state);
	lua_setglobal(state, "xpcall");

	// protolua_setup_files(state);
	protolua_setup_api(state);

	return state;
}

void protolua_setup_api(lua_State* state) {
	// TODO: Setup protologic stuff, like constants and bindings.
}
