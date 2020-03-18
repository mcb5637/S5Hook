section luaTable
        tableEntry teleportsettler,         "TeleportSettler"



section code
teleportsettler:
        ;int3
        pushad
        mov ebx, [esp+24h]
        
        push 1
        push ebx
        call shok_lua_toint         ; entityid
        
        push eax
        call shok_eid2obj
        test eax, eax
        jz .ret
        
        push eax
        
        sub esp, 8
        
        push 2
        push ebx
        call [lua_tonumber]
        add esp, 8
        fstp dword [esp] ; x
        
        push 3
        push ebx
        call [lua_tonumber]
        add esp, 8
        fstp dword [esp+4] ; y
        
        mov ecx, [esp+8]
        lea eax, [esp]
        
        ;push 0
        push eax
        call shok_teleportentity
        
        add esp, 3*4
        
        push 0
        push shok_logicMoveSettler
        push ebx
        call [lua_pushcclosure]
        add esp, 4*3
        
        push 1
        push ebx
        call [lua_insert]
        add esp, 8
        
        push 0  ; err func
        push 0  ; return values
        push 3  ; args
        push ebx
        call [lua_pcall]
        add esp, 16
        
.ret:
        popad
        mov eax, 0
        retn

