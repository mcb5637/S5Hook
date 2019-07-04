section luaTable
        tableEntry getAnimIdFromName,       "GetAnimIdFromName"

section code

getAnimIdFromName:
        pushad
        
        push -1
        push ebx
        call shok_lua_tostring
        
        mov ecx, [shok_getAnimIdFromNameObjectP]
        push eax
        call shok_getAnimIdFromName
        
        push eax
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax, 1
        ret
        