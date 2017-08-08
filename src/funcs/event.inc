section autorun
        call eventSetupFuncs

section luaTable
        tableEntry patchNetEventTrigger,    "SetNetEventTrigger"
        tableEntry unpatchNetEventTrigger,  "RemoveNetEventTrigger"

section globalVars
netEventTriggerRef      dd 0
netEventTriggerOrig     dd 0

section cleanup
        call unpatchNetEventTrigger

section code

eventFuncs:
        tableEntry eventExtractResource,    "xr"
        tableEntry event2Entities,          "e2"
        tableEntry eventEntityPos,          "ep"
        tableEntry eventEntity,             "e"
        tableEntry eventEntityPlayer,       "epl"
        tableEntry eventEntityInt,          "ei"
        tableEntry eventCannonPlacer,       "cp"
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

eventEntityPos:                     ; ep(eventID, eID, px, py)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 20
        mov ebp, esp
        
        mov dword [ebp], 76DD50h      ; CNetEventEntityAndPos
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+4], eax        ; event ID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; eID1
        
        push 3
        call lua_getfloat
        fstp dword [ebp+12]     ; x
        
        push 4
        call lua_getfloat
        fstp dword [ebp+16]     ; y
        
        push ebp
        call 525D77h
        
        add esp, 20+4
        
        popad
        xor eax, eax
        retn

eventEntity:                     ; e(eventID, eID)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 12
        mov ebp, esp
        
        mov dword [ebp], 766C28h      ; CNetEventEntityId
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+4], eax        ; event ID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; eID1
        
        push ebp
        call 525D77h
        
        add esp, 12+4
        
        popad
        xor eax, eax
        retn

eventEntityPlayer:                     ; epl(eventID, player, eID)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 16
        mov ebp, esp
        
        mov dword [ebp], 766C38h      ; CNetEventEntityIDAndPlayerID
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+4], eax        ; event ID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; player
        
        push 3
        push ebx
        call shok_lua_toint
        mov [ebp+12], eax        ; eID
        
        push ebp
        call 525D77h
        
        add esp, 16+4
        
        popad
        xor eax, eax
        retn

eventEntityInt:                     ; epl(eventID, eId, int)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 16
        mov ebp, esp
        
        mov dword [ebp], 766C48h      ; CNetEventEntityIDAndInteger
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+4], eax        ; event ID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; eID
        
        push 3
        push ebx
        call shok_lua_toint
        mov [ebp+12], eax        ; int
        
        push ebp
        call 525D77h
        
        add esp, 16+4
        
        popad
        xor eax, eax
        retn

eventCannonPlacer:                     ; cp(eId, bottom, top, px, py)
        pushad
        mov ebx, [esp+24h]
        
        sub esp, 40
        mov ebp, esp
        
        mov dword [ebp], 7705ECh      ; CNetEventCannonCreator
        mov dword [ebp+4], 69679      ; eventID
        
        push 1
        push ebx
        call shok_lua_toint
        mov [ebp+8], eax        ; eID
        
        push 2
        push ebx
        call shok_lua_toint
        mov [ebp+12], eax        ; bottom
        
        push 3
        push ebx
        call shok_lua_toint
        mov [ebp+16], eax        ; top
        
        push 4
        call lua_getfloat
        fstp dword [ebp+20]     ; x
        mov eax, [ebp+20]
        mov [ebp+32], eax
        
        push 5
        call lua_getfloat
        fstp dword [ebp+24]     ; y
        mov eax, [ebp+24]
        mov [ebp+36], eax
        
        mov dword [ebp+28], 0
        
        push ebp
        call 525D77h
        
        add esp, 40+4
        
        popad
        xor eax, eax
        retn

patchNetEventTrigger:
        pushad
        
        call unpatchNetEventTrigger
        
        ; store first arg as trigger func
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_ref]
        mov [netEventTriggerRef], eax
        add esp, 8
        
        ; get GUI_Mgr
        call 525622h
        
        ; save old func pointer
        mov eax, [eax+28h]
        mov ecx, [eax]
        mov eax, [ecx]
        mov [netEventTriggerOrig], eax
        
        ; new func pointer
        mov dword [ecx], netEventTrigger
        
        popad
        mov eax, 0
        retn

unpatchNetEventTrigger:
        pushad
        
        cmp dword [netEventTriggerRef], 0
        je .return
        
        ; get GUI_Mgr
        call 525622h
        
        ; restore func pointer
        mov eax, [eax+28h]
        mov ecx, [eax]
        mov eax, [netEventTriggerOrig]
        mov dword [ecx], eax
        
        ; clear registryindex
        push dword [netEventTriggerRef]
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_unref]
        add esp, 12
        mov dword [netEventTriggerRef], 0
        
.return:
        popad
        mov eax, 0
        retn


netEventTrigger:
        pushad
        
        mov ebx, [luaHandle]
        
        ; get func
        push dword [netEventTriggerRef]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        add esp, 12
        
        push dword [esp+28h]
        ;push ebx
        ;call shok_lua_pushint
        call daCreateUDHead ; create memory userdata
        
        mov eax, [esp+28h]
        push dword [eax+4]
        push ebx
        call shok_lua_pushint ; eventid
        
        ; call func
        push 0  ; err func
        push 0  ; return values
        push 2  ; args
        push ebx
        call [lua_pcall]
        add esp, 16
        
        popad
        jmp [netEventTriggerOrig]

