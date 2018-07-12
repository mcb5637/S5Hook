LoadLibrary equ 7611E0h
FreeLibrary equ 7611DCh


section luaTable
        tableEntry loadDLL, "LoadDLL"
        tableEntry freeDLL, "FreeDLL"
        
        
section code

loadDLL:

    pushad
    
    mov ebx, [esp + 24h]
    push 1
    push ebx
    call shok_lua_tostring
    
    test eax, eax
    jz .exitNoRet
    
    push eax
    call [LoadLibrary]
    
    push eax
    push ebx
    call shok_lua_pushint
    
    popad
    mov eax, 1
    retn
    
.exitNoRet:
    popad
    mov eax, 0
    retn
    
freeDLL:
    pushad
    mov eax, [esp + 24h]
    push 1
    push eax
    call shok_lua_toint
    push eax
    call [FreeLibrary]
    mov ebx, [esp + 24h]
    push eax
    push ebx
    call shok_lua_pushint
    popad
    mov eax, 1
    retn