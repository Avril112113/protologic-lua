#!/bin/bash
set -e

# From https://gist.github.com/lukechilds/a83e1d7127b78fef38c2914c4ececc3c
gh_get_latest_release_version() {
	curl --silent "https://api.github.com/repos/Avril112113/protologic-lua/releases?per_page=1" | # Get latest release from GitHub api
		grep '"tag_name":' |                                            # Get tag line
		sed -E 's/.*"([^"]+)".*/\1/'                                    # Pluck JSON value
}

py_get_latest_release_version() {
	grep 'VERSION = ' protolua.py |
		sed -E 's/.*"([^"]+)".*/\1/'
}

echo - Checking version has changed

if [[ $(gh_get_latest_release_version) == $(py_get_latest_release_version) ]]; then
echo Please update version number in protolua.py
exit -1
fi

echo

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
echo WARN: Windows binary for \'protolua.py\' will need to be built on windows or re-run this in wsl!
fi

cd ./release && zip -r ProtoLua.zip ./ProtoLua/
