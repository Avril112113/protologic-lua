// DO NOT MODIFY, THIS FILE IS GENERATED //
// VERSION 0.0.2 //


#pragma once

#include "_import.h"
#include "Vector3.h"
#include "RadarTargetInfo.h"


// queries.h


WASM_IMPORT("protologic", "cpu_get_fuel", cpu_get_fuel, int64_t);

WASM_IMPORT("protologic", "ship_get_position_x", ship_get_position_x, float);

WASM_IMPORT("protologic", "ship_get_position_y", ship_get_position_y, float);

WASM_IMPORT("protologic", "ship_get_position_z", ship_get_position_z, float);

WASM_IMPORT("protologic", "ship_get_position_ptr", ship_get_position_ptr, void, Vector3* dst);

WASM_IMPORT("protologic", "ship_get_velocity_x", ship_get_velocity_x, float);

WASM_IMPORT("protologic", "ship_get_velocity_y", ship_get_velocity_y, float);

WASM_IMPORT("protologic", "ship_get_velocity_z", ship_get_velocity_z, float);

WASM_IMPORT("protologic", "ship_get_velocity_ptr", ship_get_velocity_ptr, void, Vector3* dst);

WASM_IMPORT("protologic", "ship_get_orientation_x", ship_get_orientation_x, float);

WASM_IMPORT("protologic", "ship_get_orientation_y", ship_get_orientation_y, float);

WASM_IMPORT("protologic", "ship_get_orientation_z", ship_get_orientation_z, float);

WASM_IMPORT("protologic", "ship_get_orientation_w", ship_get_orientation_w, float);

WASM_IMPORT("protologic", "ship_get_orientation_ptr", ship_get_orientation_ptr, void, Vector3* dst);

WASM_IMPORT("protologic", "ship_get_angularvelocity_x", ship_get_angularvelocity_x, float);

WASM_IMPORT("protologic", "ship_get_angularvelocity_y", ship_get_angularvelocity_y, float);

WASM_IMPORT("protologic", "ship_get_angularvelocity_z", ship_get_angularvelocity_z, float);

WASM_IMPORT("protologic", "ship_get_angularvelocity_ptr", ship_get_angularvelocity_ptr, void, Vector3* dst);

WASM_IMPORT("protologic", "engine_get_fuel_amount", engine_get_fuel_amount, float);

WASM_IMPORT("protologic", "engine_get_fuel_capacity", engine_get_fuel_capacity, float);

WASM_IMPORT("protologic", "engine_get_throttle", engine_get_throttle, float);

WASM_IMPORT("protologic", "radar_get_target_count", radar_get_target_count, int32_t);

WASM_IMPORT("protologic", "radar_get_target_distance", radar_get_target_distance, float, int32_t index);

WASM_IMPORT("protologic", "radar_get_target_type", radar_get_target_type, int32_t, int32_t index);

WASM_IMPORT("protologic", "radar_get_target_id", radar_get_target_id, int64_t, int32_t index);

WASM_IMPORT("protologic", "radar_get_target_info", radar_get_target_info, void, int32_t index, RadarTargetInfo* ptr);

WASM_IMPORT("protologic", "radar_get_target_list", radar_get_target_list, void, RadarTargetInfo* ptr, int32_t len);

WASM_IMPORT("protologic", "gun0_get_bearing", gun0_get_bearing, float);

WASM_IMPORT("protologic", "gun0_get_elevation", gun0_get_elevation, float);

WASM_IMPORT("protologic", "gun0_get_refiretime", gun0_get_refiretime, float);

WASM_IMPORT("protologic", "gun1_get_bearing", gun1_get_bearing, float);

WASM_IMPORT("protologic", "gun1_get_elevation", gun1_get_elevation, float);

WASM_IMPORT("protologic", "gun1_get_refiretime", gun1_get_refiretime, float);

WASM_IMPORT("protologic", "gun2_get_bearing", gun2_get_bearing, float);

WASM_IMPORT("protologic", "gun2_get_elevation", gun2_get_elevation, float);

WASM_IMPORT("protologic", "gun2_get_refiretime", gun2_get_refiretime, float);

WASM_IMPORT("protologic", "gun3_get_bearing", gun3_get_bearing, float);

WASM_IMPORT("protologic", "gun3_get_elevation", gun3_get_elevation, float);

WASM_IMPORT("protologic", "gun3_get_refiretime", gun3_get_refiretime, float);

