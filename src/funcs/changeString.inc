section luaTable
        tableEntry changeString,    "ChangeString"
        
section code
changeString:
        push 2
        push ebx
        call [lua_tostring]
        add esp, 8
        
        push eax    
        call strdup             ; yep, small memory leak - so what
        pop ecx
        push eax
        
        push 1
        push ebx
        call [lua_tostring]
        add esp, 8
        
        push eax
        call getStringValue
        ; [[esp-0Ch]+18h]+4*ecx : ptr to string
        
        pop edx
        pop edx ; new string duplicate
        
        mov eax, [esp-14h]      ; 14h = 0Ch+8
        mov eax, [eax+18h]
        mov [eax+4*ecx], edx
        
        
        xor eax, eax            ; 0 args to pass to lua script
        retn