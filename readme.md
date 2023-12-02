# ProtoLua
ProtoLua, Lua in [ProtoLogic](https://github.com/Protologic/).  
There are few workarounds you need to keep in-mind when using ProtoLua.  


## Important to keep in mind  
Lua errors are not handled the same, they are not recoverable within Lua!  
If an error occurs, it will be printed on the next tick, before `tick()` is run.  
`pcall`, `xpcall` and `coroutine` are not available.  

Lua is initially run outside of the protologic sim to load everything.  
`init()` and `tick()` are called when running inside the protologic sim.  
When outside of the protologic sim, you have access to the filesystem (only `./lua/`).  
When inside of the protologic sim, you no longer have a filesystem.  
Code should still be system safe despite this, though there are no guarantees.  


## Usage
Only if you want to contribute to this tool, read the [Contribution](#Contribution) section.  

There is a single release which has binaries/executables for both Windows and Linux.  

Download the latest release of the tool from [Releases](https://github.com/Avril112113/protologic-lua/releases).  
Extract the `.zip`.  
Optionally, add the directory to your PATH so you can access it easily.  

If on Linux, use `protolua` instead of `protolua.exe`.  

When running `protolua.exe` with any action, it will automatically download [needed tools](#Automatically-downloaded-tools) from github.  
To create a new project run `protolua.exe create myfleet`  
To build the new project, enter the project directory and run `protolua.exe build`  
To build, sim and preview your fleet, run `protolua.exe build --fast --sim --play`  
`--fast` makes the build faster, at the cost of a un-optimized wasm file.  

**NOT IMPLEMENTED YET**  
~~To check for updates for `protolua` or `protologic`, run `protolua.exe update`~~  


## Contribution
### If you are modifying Lua:
You do not need to install any tools, just make sure to properly test any changes.  


### If you are modifying C/C++:  
You will need `wasi-sdk` and `cmake`.  
If on windows, use `wsl` and ensure all files are on the wsl filesystem to speed up build times.  

Update `WASI_SDK_PATH` in `build.sh`, don't commit this change.  
Run `./build.sh` to build `protologic.wasm`.  
Copy `./lua_template/` as `./lua/`.  
Run `./test.sh` to test using local files in `./lua/` (you will need to create `./lua/`)  
To simplify `clear && ./build.sh && ./test.sh`.  


## Detailed explanation: Lua `error`, `pcall`, `xpcall` and `coroutine`.
Lua errors are not handled as you might expect, `pcall` and `xpcall` do not recover from errors.  
This is a WASM limitation, without the ability to have support code in the host environment there is no way to properly implement `setjmp`/`longjmp`.  
So, when `longjmp` does get called by Lua, it just aborts instead, then the error is printed at the begging of the next tick.  
`coroutine`, `pcall` and `xpcall` are not accessible for the same reason, because `setjmp`/`longjmp`.  


## Automatically downloaded tools
Protolua will automatically download needed tools to build your fleet.  
These are the following;  
[WebAssembly/binaryen](https://github.com/WebAssembly/binaryen)  
[bytecodealliance/wizer](https://github.com/bytecodealliance/wizer)  
[Protologic/Release](https://github.com/Protologic/Release)  
