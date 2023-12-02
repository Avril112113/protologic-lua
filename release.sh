#!/bin/bash
set -e

rm -f -r ./release/

echo - Copying files to ./release/

mkdir ./release/
cp protolua.py ./release/protolua.py
cp -r lua_template ./release/lua_template
cp -r lua_typing ./release/lua_typing

mkdir ./release/build/
cp build/protolua.wasm ./release/build/protolua.wasm

echo

echo - Building \'protolua.py\' for Linux
pyinstaller --workpath ./build/protolua-linux --specpath ./build/ -F protolua.py
cp ./dist/protolua ./release/protolua

echo

BUILT_WINDOWS=0
if test -f /proc/sys/fs/binfmt_misc/WSLInterop; then
echo - Building \'protolua.py\' for Windows \(WSL environment\)
BUILT_WINDOWS=1
pyinstaller.exe --workpath ./build/protolua-win --specpath ./build/ -F protolua.py
cp dist/protolua.exe ./release/protolua.exe
fi

echo

echo - Built ./release/
if [[ $BUILT_WINDOWS -ne 1 ]]; then
echo WARN: Windows binary for \'protolua.py\' will need to be built on windows or wsl!
fi
