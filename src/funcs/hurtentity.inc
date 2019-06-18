section autorun
        call hurtSetup

section luaTable
        tableEntry hurtGetPointer,              "HurtEntityTrigger_DBG_GetPointer"
        tableEntry hurtResetPointer,            "HurtEntityTrigger_Reset"
        tableEntry hurtGetDamage,               "HurtEntityTrigger_GetDamage"
        tableEntry hurtSetDamage,               "HurtEntityTrigger_SetDamage"
        tableEntry hurtGetSource,               "HurtEntityTrigger_GetSource"
        tableEntry setHurtCb,                   "SetHurtEntityCallback"
        tableEntry removeHurtCb,                "RemoveHurtEntityCallback"
        tableEntry callHurtEntity,              "EntityAttackTarget"
        tableEntry getCallHurtEntitySource,     "GetHookAttackEntitySource"

section globalVars
hurtPointer      dd 0
hurtSource       dd 0
hurtCbLuaFunc    dd 0

section cleanup
        call hurtCleanup
        call removeHurtCb

section code

hurtCopied:
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        

hurtToCopy:
        nop
        push hurtJumpTo
        ret
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop

hurtJumpTo:
        mov [hurtPointer], esp ; copy pointer to data
        mov [hurtSource], esi ; copy source (entity/arrow)
        
        pushad ; check hurt cb and call it
        mov eax, [hurtCbLuaFunc]
        test eax, eax
        jz .nocb
        
        mov ebx, [luaHandle]
        
        push dword [hurtCbLuaFunc]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        add esp, 3*4
        
        mov eax, [hurtPointer]
        mov eax, [eax+4]
        test eax, eax ; attacker might be invalid
        jz .noattacker
        push dword [eax+8]
        jmp .attacker
.noattacker:
        push 0
.attacker:
        push ebx
        call shok_lua_pushint
        
        mov eax, [hurtPointer]
        mov eax, [eax+8]
        push dword [eax+8]
        push ebx
        call shok_lua_pushint
        
        push 0                  ; no err func
        push 0                  ; no return values
        push 2                  ; 2 args
        push ebx
        call [lua_pcall]
        add esp, 4*4
        
        
        
.nocb:
        popad
        
        mov eax, 27D687h ; original code that got replaced
        call 5C299Ch
        sub esp, 88h
        
        push 49F368h ; jump back
        ret

hurtSetup:
        pushad
        
        mov eax, [49F358h]
        mov [hurtCopied], eax
        
        mov eax, [49F358h+4]
        mov [hurtCopied+4], eax
        
        mov eax, [49F358h+8]
        mov [hurtCopied+8], eax
        
        mov eax, [49F358h+0Ch]
        mov [hurtCopied+0Ch], eax
        
        mov eax, [hurtToCopy]
        mov [49F358h], eax
        
        mov eax, [hurtToCopy+4]
        mov [49F358h+4], eax
        
        mov eax, [hurtToCopy+8]
        mov [49F358h+8], eax
        
        mov eax, [hurtToCopy+0Ch]
        mov [49F358h+0Ch], eax
        
        popad
        retn

hurtCleanup:
        pushad
        
        mov eax, [hurtCopied]
        mov [49F358h], eax
        
        mov eax, [hurtCopied+4]
        mov [49F358h+4], eax
        
        mov eax, [hurtCopied+8]
        mov [49F358h+8], eax
        
        mov eax, [hurtCopied+0Ch]
        mov [49F358h+0Ch], eax
        
        popad
        retn


hurtGetPointer:
        pushad
        
        push dword [hurtPointer]
        call daCreateUDHead
        
        popad
        mov eax,1
        retn

hurtResetPointer:
        pushad
        
        mov dword [hurtPointer], 0
        mov dword [hurtSource], 0
        
        popad
        mov eax,0
        retn


hurtGetDamage:
        pushad
        
        mov ebx, [esp+24h]  ; get lua state
        
        mov eax, [hurtPointer] ; get damage
        mov eax, [eax+12]
        
        push eax    ; push damage
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax,1
        retn

hurtSetDamage:
        pushad
        
        mov ebx, [esp+24h]  ; get lua state
        
        push 1      ; get new damage (arg1)
        push ebx
        call shok_lua_toint
        mov ebx, eax    ; save it
        
        mov eax, [hurtPointer] ; override it
        mov [eax+12], ebx
        
        popad
        mov eax,0
        retn

hurtGetSource:
        pushad
        
        mov ebx, [esp+24h]  ; get lua state
        
        mov eax, [hurtPointer] ; get source (return adress)
        mov eax, [eax]
        
        push eax    ; push it
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax,1
        retn


setHurtCb:
        pushad
        call removeHurtCb
        
        push LUA_REGISTRYINDEX ; remember func
        push ebx
        call [luaL_ref]
        mov [hurtCbLuaFunc], eax
        add esp, 8
        
        popad
        mov eax, 0
        ret


removeHurtCb:
        pushad
        mov eax, [hurtCbLuaFunc]
        test eax, eax
        jz .ghcbret
        
        push eax                    ; free name table reference
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_unref]
        add esp, 3*4
        
        mov dword [hurtCbLuaFunc], 0
        
.ghcbret:
        popad
        mov eax, 0
        ret


callHurtEntity:
        pushad
        mov ebx, [esp+24h]
        ;int3
        
        ; get damage and push it onto stack (arg3)
        push 3
        push ebx
        call shok_lua_toint
        push eax
        
        ; target id (arg2)
        push 2
        push ebx
        call shok_lua_toint
        push eax
        call shok_eid2obj
        push eax
        
        ; attacker id (arg1)
        push 1
        push ebx
        call shok_lua_toint
        push eax
        call shok_eid2obj ; returns 0 on invalid entity
        push eax
        
        call 49F358h
callHurtEntitySourceAdr:
        add esp, 4*3
        
        popad
        mov eax, 0
        ret

getCallHurtEntitySource:
        pushad
        mov ebx, [esp+24h]
        
        push callHurtEntitySourceAdr
        push ebx
        call shok_lua_pushint

        popad
        mov eax, 1
        ret
