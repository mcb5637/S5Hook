section luaTable
        tableEntry addArchive,      "AddArchive"
        
section code
addArchive:
        pushad
        push 1
        push ebx
        call [lua_tostring]
        
        push 1
        push eax
        call addBBArchive
        
        push 2
        push ebx
        call [lua_toboolean]
        add esp, 24
        
        test eax, eax
        jz .noPatch
        call moveLastArchiveToTop
        
.noPatch:
        popad
        xor eax, eax
        retn
        
moveLastArchiveToTop:
        mov dword esi, [88F088h]
        lea ecx, [esi+4]
        call 5BC0BDh            ; result (num elements) in eax
        mov edi, eax            ; counter in edi
        dec edi                 ; start with 2nd last elm
        
        mov eax, [esi+8]        ; base address
        push dword [eax+edi*4]  ; store last (newest) entry

.copyLoop:                      ; move all elements down by one
        mov ecx, [eax+edi*4-4]  
        mov [eax+edi*4], ecx
        dec edi
        jnz .copyLoop
        
        pop dword [eax]         ; insert last entry in front
        retn