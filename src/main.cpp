#include "global.hpp"

#include "protolua.hpp"
#include "utils.hpp"

#include <wizer.h>


lua_State* state;
static bool state_error = false;


static void init() {
	// This is run in wizer, which is outside of protologic.
	// We have access to the real file system, to load all lua files ahead of time.
	// Using wizer also allows users of protolua to not need to compile the entire project,
	// only needing bineryen and wizer, instead of everything for compiling cpp -> wasm.

	state = protolua_init();
	if (luaL_loadfile(state, "init.lua") != LUA_OK) {
		debug("Failed to load \"init.lua\"\n");
		debug("%s\n", lua_tostring(state, -1));
		abort();
	}
	// No `lua_pcall`, because custom `longjmp` causes an abort (unreachable trap).
	// There is no way to recover the error since we never continue after an abort() in wizer, but we do in protologic.
	// Still, no traceback :/
	lua_call(state, 0, LUA_MULTRET);
	lua_pop(state, lua_gettop(state));  // Clear stack of any return values.
}
WIZER_INIT(init);


extern "C" int main(int argc, char *argv[]) {}


extern "C" void tick() {
	if (state_error) {
		state_error = false;
		if (lua_isthread(state, -1)) {
			lua_State* co = lua_tothread(state, -1);
			protolua_dumpstack(co);
			lua_remove(state, -1);
			debug("\nthread error");
		} else {
			lua_remove(state, -2);
			protolua_stacktrace(state);
			debug("\n%s", lua_tostring(state, -1));
		}
		// Clean up the lua stack, it's got random stuff on it after the exception.
		lua_resetthread(state);
	}

	static bool HAS_RUN_INIT = false;
	if (!HAS_RUN_INIT) {
		HAS_RUN_INIT = true;
		protolua_setup_api(state);
		if (lua_getglobal(state, "init") != 0 && lua_isfunction(state, -1)) {
			lua_pcall(state, 0, LUA_MULTRET, 0);
		}
	}

	static bool MISSING_TICK = false;
	lua_getglobal(state, "tick");
	if (!lua_isfunction(state, -1)) {
		if (!MISSING_TICK) {
			debug("WARNING: Missing `tick()` global");
			MISSING_TICK = true;
		}
	} else {
		lua_pcall(state, 0, LUA_MULTRET, 0);
	}
	lua_pop(state, lua_gettop(state));  // Clear stack of any return values.
}

extern "C" void trap_handler(TrapCode trap_code) {
	fflush(stdout);
	if (trap_code == TrapCode::Unreachable && (get_longjmp_env() != NULL || get_longjmp_status() != 0)) {
		debug("A Lua error occurred! These are not properly supported, expect strange behaviour.");
		// debug("longjmp(%p, %d)", get_longjmp_env(), get_longjmp_status());
		state_error = true;
		reset_longjmp();
	} else {
		debug("!!!TRAP %d!!! %s", trap_code, TrapCodeToString(trap_code));
	}
}
