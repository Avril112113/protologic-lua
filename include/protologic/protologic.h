#pragma once

#include <stdio.h>

#ifdef __EMSCRIPTEN__
#define WASM_EXPORT extern "C" EMSCRIPTEN_KEEPALIVE
#else
// wasi-sdk exports are defined in CMakeLists.txt
#define WASM_EXPORT extern "C"
#endif


#define debug(...) printf(__VA_ARGS__); fflush(stdout)

#include "actions.h"
#include "constants.h"
#include "queries.h"
#include "special.h"

#include "enums.h"
#include "macros.h"
#include "consts.h"


// void main();

void trap_handler(enum TrapCode trap_code);
