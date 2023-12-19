// DO NOT MODIFY, THIS FILE IS GENERATED //
// VERSION 0.2.0 //

// Generated lua library utilising the generated C bindings.
// This NOT ready-made, is to be copy-pasted.

#include "lua_globals.hpp"

#include "protologic/protologic.hpp"


void lua_protologic_set_globals(lua_State* state) {
	lua_pushnumber(state, const_get_fueldensity());
	lua_setglobal(state, "FUELDENSITY");

	lua_pushnumber(state, const_get_spaceshipthrust());
	lua_setglobal(state, "SPACESHIPTHRUST");

	lua_pushnumber(state, const_get_spaceshipfuelconsumption());
	lua_setglobal(state, "SPACESHIPFUELCONSUMPTION");

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