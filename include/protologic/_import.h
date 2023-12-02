#pragma once

#include <stdint.h>
#include "enums.h"

#ifdef __wasm__
#define WASM_IMPORT(MODULE, NAME) __attribute__((import_module(MODULE), import_name(#NAME)))
#define WASI_IMPORT(FUNC_NAME, WASI_NAME, FRTYPE, ...) WASM_IMPORT("wasi_snapshot_preview1", WASI_NAME) extern FRTYPE FUNC_NAME(__VA_ARGS__)
#define PL_IMPORT(NAME, FRTYPE, ...) WASM_IMPORT("protologic", NAME) extern FRTYPE NAME(__VA_ARGS__)
#else
// Makes the compiler shut up
#define WASM_IMPORT(MODULE, NAME)
#define WASI_IMPORT(FUNC_NAME, WASI_NAME, FRTYPE, ...) extern FRTYPE FUNC_NAME(__VA_ARGS__)
#define PL_IMPORT(NAME, FRTYPE, ...) extern FRTYPE NAME(__VA_ARGS__)
#endif
