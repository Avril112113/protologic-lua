cmake_minimum_required(VERSION 3.17.0)

project(protologic_lua)


add_link_options(-Wl,--export=tick)
add_link_options(-Wl,--export=trap_handler)
add_link_options(-fno-exceptions)
add_link_options(-z stack-size=4194304)

add_compile_options(-fno-exceptions)
add_compile_options(-Wno-deprecated-declarations)

# These defines are probably not needed.
add_compile_definitions(_WASM_)
add_compile_definitions(_WASI_EMULATED_SIGNAL)
add_compile_definitions(_WASI_EMULATED_PROCESS_CLOCKS)
add_compile_definitions(L_tmpnam=260)

add_subdirectory("./lib/lua")
add_subdirectory("./lib/fill")

include_directories("./src")
include_directories("./include")
include_directories("./include/lua-5.4.6")

include_directories("./lib/fill")

file(GLOB_RECURSE SOURCES src/*.c src/*.cpp)

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
add_executable(protolua ${SOURCES})
target_link_libraries(protolua fill)
target_link_libraries(protolua lua)
