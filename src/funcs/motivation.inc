section luaTable
        tableEntry setSMotivation,          "SetSettlerMotivation"
        tableEntry getSettlerMaxWorktime,   "GetSettlerMaxWorktime"

section code
setSMotivation:
        pushad
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax
        call shok_eid2obj
        test eax, eax
        jz .noentity
        
        lea ecx, [eax+0B4h]         ;this
        
        mov ebx, 13007h             
        push ebx
        push ebx
        mov ebx, esp
        push ebx
        add ebx, 4
        push ebx
        call 58403Eh
        
        mov edi, [ecx+10h]
        mov edi, [edi+4]
        
        push 2
        push esi
        call [lua_tonumber]
        fstp dword [edi+18h]
        
        add esp, 10h
        
.noentity:
        popad
        xor eax, eax
        retn
        
        
searchEntityBehavior:           ; ecx->this->entityobject, a1->behavior class
        mov eax, [esp+4]     ; class to search
        
        push ebx
        push ecx
        push edx
        
        mov ebx, [ecx+shok_EGL_CGLEEntityOO_BehaviorListStart] ; start of behaviorlist
        mov ecx, [ecx+shok_EGL_CGLEEntityOO_BehaviorListEnd] ; end of behaviorlist
        
.loopstart:
        cmp ecx, ebx
        jle .return0
        
        mov edx, [ebx]
        cmp edx, 0      ; valid entry
        je .next
        
        cmp [edx], eax      ; compare class
        jne .next
        
        mov eax, edx
        jmp .return
        
.next:
        add ebx, 4
        jmp .loopstart
        
.return0:
        mov eax, 0

.return:
        pop edx
        pop ecx
        pop ebx
        retn 4
        
        
getSettlerMaxWorktime:
        pushad
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax
        call shok_eid2obj
        test eax, eax
        jz .noentity
        
        mov ecx, eax
        push shok_GGL_CWorkerBehaviorVt
        call searchEntityBehavior
        test eax, eax
        jz .noentity
        
        mov ecx, eax        ; this
        call shok_GGL_CWorkerBehavior_GetMaxWorktimeAdr
        
        push eax
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax, 1
        retn
        
        
.noentity:
        popad
        xor eax, eax
        retn
        