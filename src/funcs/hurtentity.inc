section autorun
        call hurtSetup

section luaTable
        tableEntry hurtGetPointer,    "HurtEntityTrigger_DBG_GetPointer"
        tableEntry hurtResetPointer,  "HurtEntityTrigger_Reset"
        tableEntry hurtGetDamage,     "HurtEntityTrigger_GetDamage"
        tableEntry hurtSetDamage,     "HurtEntityTrigger_SetDamage"
        tableEntry hurtGetSource,     "HurtEntityTrigger_GetSource"

section globalVars
hurtPointer      dd 0
hurtSource       dd 0

section cleanup
        call hurtCleanup

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
        
        mov eax, [hurtSource] ; get source
        mov eax, [eax]
        
        push eax    ; push it
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax,1
        retn


