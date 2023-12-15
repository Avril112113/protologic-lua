#!/bin/bash

# Build protolua.wasm

WASI_SDK_PATH=/home/dude/wasi-sdk-20.0

mkdir -p ./build

rm -f ./build/*.wasm
rm -f ./build/*.wat

echo \~ Building
cmake --toolchain ${WASI_SDK_PATH}/share/cmake/wasi-sdk.cmake -Bbuild
( cd ./build && make )

# Downloaded by protolua
echo \~ Running wasm-opt on ./build/protolua.wasm
tools/binaryen/bin/wasm-opt ./build/protolua.wasm -o ./build/protolua.wasm --strip-dwarf --asyncify --enable-simd --enable-bulk-memory

echo
