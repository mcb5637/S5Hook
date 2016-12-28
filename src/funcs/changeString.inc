section luaTable
        tableEntry changeString,    "ChangeString"
        
section code
changeString:
        push 2
        push ebx
        call [lua_tostring]
        add esp, 8
        push eax    ; new name, [1]

        push 1
        push ebx
        call [lua_tostring]
        add esp, 8
        
        push eax    ; path
        call getStringValue 
        ; [[esp-0Ch]+18h]+4*ecx : ptr to string
        test eax, eax
        pop edx
        pop eax ; new name [1]
        jz .notfound
        
        
        mov edx, [esp-14h]      ; 14h = 0Ch+8
        mov edx, [edx+18h]
        lea edx, [edx+4*ecx]
        push edx                ; string table ptr
        
        ; ignore ptr to old name, - memory leak - so what
        push eax
        call strdup             
        pop edx
        
        pop edx         ; string table ptr
        mov [edx], eax

        
.notfound:
        xor eax, eax            ; 0 args to pass to lua script
        retn