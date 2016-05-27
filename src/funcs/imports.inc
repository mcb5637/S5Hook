section autorun
        call fetchImports


section code

sS5Lua5     db "s5lua5", 0
sSetMetaT   db "lua_setmetatable", 0
sNewUserD   db "lua_newuserdata", 0
sLuaL_err   db "luaL_error", 0

fetchImports:
        push sS5Lua5
        call [GetModuleHandleA]
        
        push sLuaL_err
        push eax
        
        push sNewUserD
        push eax
        
        push sSetMetaT
        push eax
        
        call [GetProcAddress]
        mov dword [lua_setmetatable], eax
        
        call [GetProcAddress]
        mov dword [lua_newuserdata], eax
        
        call [GetProcAddress]
        mov dword [luaL_error], eax
        retn