# S5Hook

S5Hook is a extension for Settlers 5 which can be loaded at runtime through the Lua script embedded in a usermap.

### Usage
Include the code from `output/S5Hook.lua` in your map and execute `InstallHook(callback)`. Installation takes a few Game-Turns, but your callback function will be called when S5Hook was successfully loaded. All functions reside in a global table call `S5Hook`. All available functions and their usage are documented in the Lua file.
You also need to load the [BigNum](http://oss.digirati.com.br/luabignum/bn/) library, which is neccessary to work around the buggy implementation in S5 Lua.

### Building
Simply execute the `build.bat` to build the source code into a ready-to-use Lua script.
Well almost, if you make changes to the loader, you have to add these changes manually into the Lua file using the ``utils/generator.exe`` application.

### Download
Get the latest version of:
 - [S5Hook](https://bitbucket.org/dbeinder/s5hook/raw/tip/output/S5Hook.lua)
 - [BigNum](https://bitbucket.org/dbeinder/s5hook/raw/tip/utils/BigNum.lua)
Download with Alt+Click!