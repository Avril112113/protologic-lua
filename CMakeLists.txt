cmake_minimum_required(VERSION 3.17.0)

project(protologic_lua)

add_compile_definitions(_WASM_)
add_link_options(-Wl,--export=tick)
add_link_options(-Wl,--export=trap_handler)
add_compile_options(-fno-exceptions)
add_link_options(-fno-exceptions)


# These defines are probably not needed.
add_compile_definitions(_WASI_EMULATED_SIGNAL)
add_compile_definitions(_WASI_EMULATED_PROCESS_CLOCKS)
add_compile_definitions(LUA_USE_C89)
add_compile_definitions(L_tmpnam=260)
add_subdirectory("./lib/lua")

include_directories("./src")
include_directories("./lua_c")
include_directories("./include")
include_directories("./include/lua-5.4.6")

include_directories("./lib/fill")

file(GLOB_RECURSE SOURCES src/*.c src/*.cpp lua_c/*.cpp)

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
add_executable(protolua ${SOURCES})
target_link_libraries(protolua lua)
