#!/bin/bash

# Result from https://github.com/Avril112113/protologic_bgen/tree/main

BGEN_OUT_PATH="/mnt/c/Users/dude/Desktop/ProgrammingProjects/python/Work In Progress/protologic_bgen/out"

find "$BGEN_OUT_PATH/C/" -name "*.h" -exec cp {} ./include/protologic \;
find "$BGEN_OUT_PATH/Lua/" -name "*.lua" -exec cp {} ./lua_typing \;
find "$BGEN_OUT_PATH/Lua/Cpp/" -name "*" -exec cp {} ./src/protologic \;
