#!/bin/bash
set -e

rm -f -r ./release/
mkdir ./release/

echo - Copying files to ./release/ProtoLua/

mkdir ./release/ProtoLua/
cp protolua.py ./release/ProtoLua/protolua.py
cp -r lua_template ./release/ProtoLua/lua_template
cp -r lua_typing ./release/ProtoLua/lua_typing

mkdir ./release/ProtoLua/build/
cp build/protolua.wasm ./release/ProtoLua/build/protolua.wasm

echo

echo - Building \'protolua.py\' for Linux
pyinstaller --workpath ./build/protolua-linux --specpath ./build/ -F protolua.py
cp ./dist/protolua ./release/ProtoLua/protolua

echo

BUILT_WINDOWS=0
if test -f /proc/sys/fs/binfmt_misc/WSLInterop; then
echo - Building \'protolua.py\' for Windows \(WSL environment\)
BUILT_WINDOWS=1
pyinstaller.exe --workpath ./build/protolua-win --specpath ./build/ -F protolua.py
cp dist/protolua.exe ./release/ProtoLua/protolua.exe
fi

echo

echo - Built ./release/ProtoLua/
if [[ $BUILT_WINDOWS -ne 1 ]]; then
echo WARN: Windows binary for \'protolua.py\' will need to be built on windows or wsl!
fi

cd ./release && zip -r ProtoLua.zip ./ProtoLua/
