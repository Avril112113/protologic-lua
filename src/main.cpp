#include "global.hpp"

#include "protolua.hpp"
#include "utils.hpp"

#include <wizer_modified.h>


lua_State* state;


static void init() {
	// This is run in wizer, which is outside of protologic.
	// We have access to the real file system, to load all lua files ahead of time.
	// Using wizer also allows users of protolua to not need to compile the entire project,
	// only needing bineryen and wizer, instead of everything for compiling cpp -> wasm.
	// Also note, we use a modified wizer.h that calls this method with setjmp/longjmp support.

	state = protolua_init();
	if (luaL_loadfile(state, "init.lua") != LUA_OK) {
		debug("Failed to load \"init.lua\"\n");
		debug("%s\n", lua_tostring(state, -1));
		abort();
	}
	if (protolua_pcall(state, 0, LUA_MULTRET) != 0) {
		debug("%s\n", lua_tostring(state, -1));
	}
	lua_pop(state, lua_gettop(state));  // Clear stack of any return values.

	// After exiting here, we will be in the protologic environment.
	// This is done here instead to save fuel on the first tick.
	protolua_setup_api_pre(state);
}
WIZER_INIT(init);


extern "C" int main(int argc, char *argv[]) {}


void _tick() {
	static bool HAS_RUN_INIT = false;
	if (!HAS_RUN_INIT) {
		HAS_RUN_INIT = true;
		protolua_setup_api_post(state);
		if (lua_getglobal(state, "init") != 0 && lua_isfunction(state, -1)) {
			if (protolua_pcall(state, 0, LUA_MULTRET) != 0) {
				debug("Error in _G.init()");
				debug("%s\n", lua_tostring(state, -1));
			}
		}
	}

	static bool MISSING_TICK = false;
	if (lua_getglobal(state, "tick") == 0 || !lua_isfunction(state, -1)) {
		if (!MISSING_TICK) {
			debug("WARNING: Missing _G.tick()");
			MISSING_TICK = true;
		}
	} else {
		if (protolua_pcall(state, 0, LUA_MULTRET) != 0) {
			debug("Error in _G.tick()");
			debug("%s\n", lua_tostring(state, -1));
		}
	}
	lua_pop(state, lua_gettop(state));  // Clear stack of any return values.
}
extern "C" void tick() {
	RUN_JMP_SUPPORT_VOID(_tick());
}

extern "C" void trap_handler(TrapCode trap_code) {
	asyncify_stop_unwind();  // Safety call, in-case the trap was during a unwind or rewind, this fixes stuff in both cases.
	fflush(stdout);
	debug("!!!TRAP %d!!! %s", trap_code, TrapCodeToString(trap_code));
	abort();  // Cause double trap.
}
