// DO NOT MODIFY, THIS FILE IS GENERATED //
// VERSION 0.0.1 //


#pragma once

#include "_import.h"


// All functions related to "doing" stuff.


PL_IMPORT(ship_self_destruct, void);

PL_IMPORT(engine_set_throttle, void, float throttle);

// Set the torque for the ship to rotate.
PL_IMPORT(wheel_set_torque, void, float x, float y, float z);

PL_IMPORT(runninglight_set_state, void, int32_t state);

PL_IMPORT(radar_set_angle, void, float angle);

PL_IMPORT(radar_set_bearing, void, float bearing);

PL_IMPORT(radar_set_elevation, void, float elevation);

PL_IMPORT(radar_trigger, void);

PL_IMPORT(gun0_set_bearing, void, float bearing);

PL_IMPORT(gun0_set_elevation, void, float elevation);

PL_IMPORT(gun0_set_fuse, void, float fuse);

PL_IMPORT(gun0_trigger, void);

PL_IMPORT(gun1_set_bearing, void, float bearing);

PL_IMPORT(gun1_set_elevation, void, float elevation);

PL_IMPORT(gun1_set_fuse, void, float fuse);

PL_IMPORT(gun1_trigger, void);

PL_IMPORT(gun2_set_bearing, void, float bearing);

PL_IMPORT(gun2_set_elevation, void, float elevation);

PL_IMPORT(gun2_set_fuse, void, float fuse);

PL_IMPORT(gun2_trigger, void);

PL_IMPORT(gun3_set_bearing, void, float bearing);

PL_IMPORT(gun3_set_elevation, void, float elevation);

PL_IMPORT(gun3_set_fuse, void, float fuse);

PL_IMPORT(gun3_trigger, void);
