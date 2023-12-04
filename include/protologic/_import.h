// DO NOT MODIFY, THIS FILE IS GENERATED //
// VERSION {{bindings.version}} //


#pragma once

#include <stdint.h>

#define WASM_IMPORT(MODULE, WASM_NAME, FUNC_NAME, FRTYPE, ...) __attribute__((import_module(MODULE), import_name(WASM_NAME))) extern FRTYPE FUNC_NAME(__VA_ARGS__)
