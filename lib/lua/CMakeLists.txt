cmake_minimum_required(VERSION 3.10.0)

project(lua)

add_compile_definitions(_WASI_EMULATED_SIGNAL)
add_compile_definitions(_WASI_EMULATED_PROCESS_CLOCKS)
add_compile_definitions(LUA_USE_C89)
add_compile_definitions(L_tmpnam=260)

# From https://github.com/walterschell/src/blob/master/lua-5.4.6/CMakeLists.txt
set(LUA_SOURCE 
    "src/lapi.c"
    "src/lcode.c"
    "src/lctype.c"
    "src/ldebug.c"
    "src/ldo.c"
    "src/ldump.c"
    "src/lfunc.c"
    "src/lgc.c"
    "src/llex.c"
    "src/lmem.c"
    "src/lobject.c"
    "src/lopcodes.c"
    "src/lparser.c"
    "src/lstate.c"
    "src/lstring.c"
    "src/ltable.c"
    "src/ltm.c"
    "src/lundump.c"
    "src/lvm.c"
    "src/lzio.c"
    "src/lauxlib.c"
    "src/lbaselib.c"
    "src/lcorolib.c"
    "src/ldblib.c"
    "src/liolib.c"
    "src/lmathlib.c"
    "src/loadlib.c"
    "src/loslib.c"
    "src/lstrlib.c"
    "src/ltablib.c"
    "src/lutf8lib.c"
    "src/linit.c"
    "../fill/setjmp.c"
    "../fill/fill.c"
)

include_directories("../fill")

file(GLOB SOURCES *.c)

add_library(lua ${LUA_SOURCE};${SOURCES})
