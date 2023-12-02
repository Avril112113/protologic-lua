#pragma once


#include "constants.h"


static float RADAR_MIN_ANGLE;
static float RADAR_MAX_ANGLE;

static float SHIP_MAX_THRUST;
static float SHIP_FUEL_CONSUMPTION;
static float SHIP_FUEL_DENSITY;
static float SHIP_BASE_MASS;
static float SHIP_WHEEL_TORQUE;

static float TURRET_SHELL_SPEED;
static float TURRET_REFIRE_TIME;

void inline InitConsts() {
	static int initialized = 0;

	if (!initialized) {
		RADAR_MIN_ANGLE = const_get_shipradarminangle();
		RADAR_MAX_ANGLE = const_get_shipradarmaxangle();

		SHIP_MAX_THRUST = const_get_spaceshipthrust();
		SHIP_FUEL_CONSUMPTION = const_get_spaceshipfuelconsumption();
		SHIP_FUEL_DENSITY = const_get_fueldensity();
		SHIP_BASE_MASS = const_get_shipbasemass();
		SHIP_WHEEL_TORQUE = const_get_shipwheeltorque();

		TURRET_SHELL_SPEED = const_get_turretshellspeed();
		TURRET_REFIRE_TIME = const_get_turretrefiretime();

		initialized = 1;
	}
};
