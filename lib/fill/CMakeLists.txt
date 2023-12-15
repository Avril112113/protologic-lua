cmake_minimum_required(VERSION 3.10.0)

project(fill)

add_compile_options(-Wno-invalid-noreturn)

add_compile_definitions(_WASI_EMULATED_SIGNAL)
add_compile_definitions(_WASI_EMULATED_PROCESS_CLOCKS)

include_directories("../../include")

file(GLOB SOURCES *.c *.cpp)

add_library(fill ${SOURCES})
