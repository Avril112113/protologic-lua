#include "global.hpp"

#include "protolua.hpp"
#include "lua_protologic.hpp"
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

	// Constants
	lua_pushnumber(state, const_get_fueldensity());
	lua_setglobal(state, "FUELDENSITY");

	lua_pushnumber(state, const_get_spaceshipthrust());
	lua_setglobal(state, "SPACESHIPTHRUST");

	lua_pushnumber(state, const_get_spaceshipfuelconsumption());
	lua_setglobal(state, "SPACESHIPFUELCONSUMPTION");

	lua_pushnumber(state, const_get_missilethrust());
	lua_setglobal(state, "MISSILETHRUST");

	lua_pushnumber(state, const_get_missilefuelconsumption());
	lua_setglobal(state, "MISSILEFUELCONSUMPTION");

	lua_pushnumber(state, const_get_turretminelevation());
	lua_setglobal(state, "TURRETMINELEVATION");

	lua_pushnumber(state, const_get_turretmaxelevation());
	lua_setglobal(state, "TURRETMAXELEVATION");

	lua_pushnumber(state, const_get_turretelevationspeed());
	lua_setglobal(state, "TURRETELEVATIONSPEED");

	lua_pushnumber(state, const_get_turretbearingspeed());
	lua_setglobal(state, "TURRETBEARINGSPEED");

	lua_pushnumber(state, const_get_turretminfuse());
	lua_setglobal(state, "TURRETMINFUSE");

	lua_pushnumber(state, const_get_turretmaxfuse());
	lua_setglobal(state, "TURRETMAXFUSE");

	lua_pushnumber(state, const_get_turretshellspeed());
	lua_setglobal(state, "TURRETSHELLSPEED");

	lua_pushnumber(state, const_get_turretrefiretime());
	lua_setglobal(state, "TURRETREFIRETIME");

	lua_pushnumber(state, const_get_missileradarminrange());
	lua_setglobal(state, "MISSILERADARMINRANGE");

	lua_pushnumber(state, const_get_missileradarmaxrange());
	lua_setglobal(state, "MISSILERADARMAXRANGE");

	lua_pushnumber(state, const_get_missileradarminangle());
	lua_setglobal(state, "MISSILERADARMINANGLE");

	lua_pushnumber(state, const_get_missileradarmaxangle());
	lua_setglobal(state, "MISSILERADARMAXANGLE");

	lua_pushnumber(state, const_get_shipradarminangle());
	lua_setglobal(state, "SHIPRADARMINANGLE");

	lua_pushnumber(state, const_get_shipradarmaxangle());
	lua_setglobal(state, "SHIPRADARMAXANGLE");

	lua_pushnumber(state, const_get_missilerefiretime());
	lua_setglobal(state, "MISSILEREFIRETIME");

	lua_pushnumber(state, const_get_shipbasemass());
	lua_setglobal(state, "SHIPBASEMASS");

	lua_pushnumber(state, const_get_missilebasemass());
	lua_setglobal(state, "MISSILEBASEMASS");

	lua_pushnumber(state, const_get_missilewheeltorque());
	lua_setglobal(state, "MISSILEWHEELTORQUE");

	lua_pushnumber(state, const_get_shipwheeltorque());
	lua_setglobal(state, "SHIPWHEELTORQUE");

	lua_pushnumber(state, const_get_shipradius());
	lua_setglobal(state, "SHIPRADIUS");

	lua_pushnumber(state, const_get_missileradius());
	lua_setglobal(state, "MISSILERADIUS");

	lua_pushinteger(state, const_get_shipmaxcpufuel());
	lua_setglobal(state, "SHIPMAXCPUFUEL");

	lua_pushinteger(state, const_get_shipmincpufuel());
	lua_setglobal(state, "SHIPMINCPUFUEL");

	lua_pushinteger(state, const_get_shippertickcpufuel());
	lua_setglobal(state, "SHIPPERTICKCPUFUEL");
}
