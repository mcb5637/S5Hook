section code

registerFuncTable:
        push esi
        push edi
        mov esi, [esp+12]               ; functable
        mov edi, [esp+16]               ; table name (string)
        
.nextEntry:
        push 0                          ; no description
        push dword [esi]                ; func ptr
        add esi, 5
        push esi                        ; func name (string)
        movzx eax, byte [esi-1]         ; skip over func name
        add esi, eax
        push edi                    ; base table
        push ebx                        ; lua handle
        call regLuaFunc
        
        cmp dword [esi], 0
        jnz .nextEntry
        
        pop edi
        pop esi
        retn 8