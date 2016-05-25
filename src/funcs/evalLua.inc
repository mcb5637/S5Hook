section luaTable
        tableEntry evalLua,         "Eval"

section code
evalLua:
        push 0                      ; no name
        
        push 1
        push ebx
        call [lua_tostring]
        add esp, 8
        
        push eax                    ; will be overwritten with  size
        push eax                    ; lua buffer
        call strlen
        
        mov [esp+4], eax            ; size
        push ebx
        call [luaL_loadbuffer]
        add esp, 16
        
        xor eax, eax                ; shorter than <mov eax, 1> ;)
        inc eax
        retn