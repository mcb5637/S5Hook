section luaTable
        tableEntry rtStore,         "RuntimeStore"
        tableEntry rtLoad,          "RuntimeLoad"

section code
rtStore:
        push esi
        mov esi, [rtState]
        
        push 1
        push ebx
        call [lua_tostring]
        
        push eax
        push esi
        call [lua_pushstring]
        
        push 2
        push ebx
        call [lua_tostring]
        
        push eax
        push esi
        call [lua_pushstring]
        
        push LUA_REGISTRYINDEX
        push esi
        call [lua_settable]
        

        add esp, 40
        
        pop esi
        xor eax, eax
        retn
        
rtLoad:
        push esi
        mov esi, [rtState]
        
        push 1
        push ebx
        call [lua_tostring]
        
        push eax
        push esi
        call [lua_pushstring]
        
        push LUA_REGISTRYINDEX
        push esi
        call [lua_gettable]
        
        push -1
        push esi
        call [lua_tostring]
        
        push eax
        push ebx
        call [lua_pushstring]

        add esp, 40
        
        pop esi
        xor eax, eax
        inc eax
        retn