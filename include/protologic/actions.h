// DO NOT MODIFY, THIS FILE IS GENERATED //
// VERSION 0.2.0 //


#pragma once

#include "_import.h"


// actions.h
// All functions related to "doing" stuff.


WASM_IMPORT("protologic", "ship_self_destruct", ship_self_destruct, void);

WASM_IMPORT("protologic", "engine_set_throttle", engine_set_throttle, void, float throttle);

// Set the torque for the ship to rotate.
WASM_IMPORT("protologic", "wheel_set_torque", wheel_set_torque, void, float x, float y, float z);

WASM_IMPORT("protologic", "runninglight_set_state", runninglight_set_state, void, int32_t state);

WASM_IMPORT("protologic", "radar_set_angle", radar_set_angle, void, float angle);

WASM_IMPORT("protologic", "radar_set_bearing", radar_set_bearing, void, float bearing);

WASM_IMPORT("protologic", "radar_set_elevation", radar_set_elevation, void, float elevation);

WASM_IMPORT("protologic", "radar_trigger", radar_trigger, void);

WASM_IMPORT("protologic", "gun0_set_bearing", gun0_set_bearing, void, float bearing);

WASM_IMPORT("protologic", "gun0_set_elevation", gun0_set_elevation, void, float elevation);

WASM_IMPORT("protologic", "gun0_set_fuse", gun0_set_fuse, void, float fuse);

WASM_IMPORT("protologic", "gun0_trigger", gun0_trigger, void);

WASM_IMPORT("protologic", "gun0_reload", gun0_reload, void, int32_t ammo);

WASM_IMPORT("protologic", "gun1_set_bearing", gun1_set_bearing, void, float bearing);

WASM_IMPORT("protologic", "gun1_set_elevation", gun1_set_elevation, void, float elevation);

WASM_IMPORT("protologic", "gun1_set_fuse", gun1_set_fuse, void, float fuse);

WASM_IMPORT("protologic", "gun1_trigger", gun1_trigger, void);

WASM_IMPORT("protologic", "gun1_reload", gun1_reload, void, int32_t ammo);

WASM_IMPORT("protologic", "gun2_set_bearing", gun2_set_bearing, void, float bearing);

WASM_IMPORT("protologic", "gun2_set_elevation", gun2_set_elevation, void, float elevation);

WASM_IMPORT("protologic", "gun2_set_fuse", gun2_set_fuse, void, float fuse);

WASM_IMPORT("protologic", "gun2_trigger", gun2_trigger, void);

WASM_IMPORT("protologic", "gun2_reload", gun2_reload, void, int32_t ammo);

WASM_IMPORT("protologic", "gun3_set_bearing", gun3_set_bearing, void, float bearing);

WASM_IMPORT("protologic", "gun3_set_elevation", gun3_set_elevation, void, float elevation);

WASM_IMPORT("protologic", "gun3_set_fuse", gun3_set_fuse, void, float fuse);

WASM_IMPORT("protologic", "gun3_trigger", gun3_trigger, void);

WASM_IMPORT("protologic", "gun3_reload", gun3_reload, void, int32_t ammo);

WASM_IMPORT("protologic", "debug_sphere_set", debug_sphere_set, void, float x, float y, float z, float radius, float r, float g, float b);

WASM_IMPORT("protologic", "debug_line_set", debug_line_set, void, float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);

