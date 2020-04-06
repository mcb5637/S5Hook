section luaTable
    tableEntry reloadCutscenes, "ReloadCutscenes"
        
section code
reloadCutscenes:

        push 1
        push ebx
        call [lua_tostring]
        add esp, 8
        
        test eax, eax
        jz .defaultDir
        push eax
        jmp .reload
        
.defaultDir:
        push shok_str_mapsexternalmap            ; ptr to "Maps\ExternalMap"
        
.reload:
        mov dword eax, [shok_reloadCutsceneObj]
        mov ecx, [eax]
        mov eax, [ecx]
    
        call [eax+shok_reloadCutsceneVtOReload]          ; 2nd arg in ecx
        
        xor eax, eax
        retn