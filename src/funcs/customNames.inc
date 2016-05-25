section luaTable
        tableEntry patchCN,         "SetCustomNames"
        tableEntry unpatchCN,       "RemoveCustomNames"

section globalVars
cnTableRef      dd 0

section cleanup
        call unpatchCN

section code
patchCN:
        mov eax, cnJump             ; create jump at this location 
        mov byte [eax], 0E9h        ; opcode jmp
        mov dword [eax+1], cnOffset ; relative jmp target
        mov byte [eax-013h], 0EBh   ; make jump unconditional

        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_ref]
        mov [cnTableRef], eax
        add esp, 8

        xor eax, eax
        retn
        
unpatchCN:
        mov eax, [cnTableRef]
        test eax, eax
        jz .notPatched
        
        push eax                    ; free name table reference
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_unref]
        add esp, 12

        mov eax, cnJump 
        mov byte [eax], 0E8h        ; reset to: <CALL 0052EE1C> 
        mov dword [eax+1], 0FFFEF506h   
        mov byte [eax-013h], 74h    ; opcode back to JE 
        
        mov dword [cnTableRef], 0
        
.notPatched:
        xor eax, eax
        retn

cnJump      equ 53F911h
cnOffset    equ customNames - (cnJump + 5)

customNames:
        mov eax, [eax+0C8h]     ; get entity name
        test eax, eax
        jz .doDefault           ; no name   
        
        push edi 
        mov edi, eax
        
        mov ebx, [luaHandle]
        
        push 0                  ; prevent the lua stack from growing indefinitely
        push ebx                
        call [lua_settop]       ; clears the stack
        
        push dword [cnTableRef]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        
        push edi
        push ebx
        call [lua_pushstring]
        
        push -2
        push ebx
        call [lua_gettable]
        
        push -1
        push ebx
        call [lua_tostring]
        
        add esp, 44
        
        pop edi
        mov ebx, 0
        cmp eax, ebx
        jnz .returnWithPop
        
.doDefault:
        call 52EFCFh
        jmp 53F91Dh
        
.returnWithPop:
        pop ecx
        jmp 53F91Dh
