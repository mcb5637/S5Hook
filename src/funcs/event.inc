section autorun
        call eventSetupFuncs

section code

eventFuncs:
        tableEntry eventExtractResource,    "xr"
        tableEntry event2Entities,          "e2"
        dd 0
        
sEventTbl     db "__event", 0

eventSetupFuncs:
        push sEventTbl
        push eventFuncs
        call registerFuncTable
        retn


eventExtractResource:           ; xr(eID, resourceType, x, y)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 24
        mov ebp, esp
        
        mov dword [ebp], 77061Ch      ; CNetEventExtractResource
        mov dword [ebp+4], 69657      ; eventID
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; serf eID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+12], eax       ; resourceType
        
        push 3
        call lua_getfloat
        fstp dword [ebp+16]     ; x
        
        push 4
        call lua_getfloat
        fstp dword [ebp+20]     ; y
        
        push ebp
        call 525D77h
        
        add esp, 24+4
        
        popad
        xor eax, eax
        retn

event2Entities:                     ; e2(eventID, eID1, eID2)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 16
        mov ebp, esp
        
        mov dword [ebp], 76DD60h      ; CNetEvent2Entities
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+4], eax        ; event ID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; eID1
        
        push 3
        push ebx
        call shok_lua_toint
        mov [ebp+12], eax       ; eID2
        
        push ebp
        call 525D77h
        
        add esp, 16+4
        
        popad
        xor eax, eax
        retn