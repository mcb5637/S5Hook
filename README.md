This is a fork (and conversion to git) from https://bitbucket.org/settlersdev/s5hook/src/default/
(bitbucket dropped hg support)

# S5Hook

S5Hook is a extension for Settlers 5 which can be loaded at runtime through the Lua script embedded in a usermap.

### Usage
Include the code from `output/S5Hook.lua` in your map and execute `InstallS5Hook()`. Starting from v1.0 the setup take place instantly - but check the return value to catch older patch versions.

### Building
Simply execute the `build.bat` to build the source code into a ready-to-use Lua script.
