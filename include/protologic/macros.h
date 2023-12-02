// For more fancy macros.

#pragma once

#include "_import.h"


#define gun_get_bearing(GUN_ID) gun##GUN_ID##_get_bearing()
#define gun_get_elevation(GUN_ID) gun##GUN_ID##_get_elevation()
#define gun_get_refiretime(GUN_ID) gun##GUN_ID##_get_refiretime()
#define gun_get_refiretime(GUN_ID) gun##GUN_ID##_get_refiretime()

#define gun_set_bearing(GUN_ID, BEARING) gun##GUN_ID##_set_bearing(BEARING)
#define gun_set_elevation(GUN_ID, ELEVATION) gun##GUN_ID##_set_elevation(ELEVATION)
#define gun_set_fuse(GUN_ID, FUSE) gun##GUN_ID##_set_fuse(FUSE)

#define gun_trigger(GUN_ID) gun##GUN_ID##_set_elevation()
