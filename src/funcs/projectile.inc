section luaTable
        tableEntry createProjectileNew,     "CreateProjectile"
        tableEntry isValidEffect,           "IsValidEffect"
        tableEntry setGlobalHitCb,          "SetGlobalProjectileHitCallback"
        tableEntry removeGlobalHitCb,       "RemoveGlobalProjectileHitCallback"
        tableEntry setEffectCreatedCb,      "SetEffectCreatedCallback"
        tableEntry removeEffectCreatedCb,   "RemoveEffectCreatedCallback"
        tableEntry getEffectObject,         "GetEffectMem"

section globalVars
globalHitCbLuaFunc              dd 0
globalEffectCreatedCbLuaFunc    dd 0
globalEffectCreatedCbOriginal   dd 0
createProjectileMarker          dd 0

section autorun
        call setGlobalHitCbHook

section cleanup
        call removeGlobalHitCb
        call removeEffectCreatedCb
        call removeGlobalHitCbHook

section code

sEffectCbsTable                 dd "__effectcbs", 0



;ex: S5Hook.CreateProjectile(12, 3500,4000,3200,2900, 10, -1, 65541, 131078, function() Message(42); end)
;S5Hook.CreateProjectile(   1   int effectType, 
                        ;   2   float startX,
                        ;   3   float startY, 
                        ;   4   float targetX, 
                        ;   5   float targetY 
                        ;   6   [, int damage = 0 
                        ;   7   [, float radius = -1 
                        ;   8   [, int targetId = 0
                        ;   9   [, int attackerId = 0
                        ;   10  [, fn hitCallback]]]])

              
createProjectileNew:
        pushad
        
        mov dword [createProjectileMarker], 1
        
        ; make sruct
        mov ebx, [esp + 24h]
        sub esp, ProjDef.size
        mov ebp, esp
        
        xor eax, eax
        mov ecx, ProjDef.size
.clear: 
        mov [ebp + ecx - 1], al
        dec ecx
        jnz .clear
        
        ;fill struct
        mov dword [ebp + ProjDef.creatorType], shok_CProjectileEffectCreator        ;CProjectileEffectCreator
        
        push 1
        push ebx
        call shok_lua_toint
        mov dword [ebp + ProjDef.effectType], eax
        
        push 2
        push esi
        call [lua_tonumber]
        add esp, 8
        fst dword [ebp + ProjDef.startX]
        fstp dword [ebp + ProjDef.startXcopy]

        push 3
        push esi
        call [lua_tonumber]
        add esp, 8
        fst dword [ebp + ProjDef.startY]
        fstp dword [ebp + ProjDef.startYcopy]

        push 4
        push esi
        call [lua_tonumber]
        fstp dword [ebp + ProjDef.targetX]
        add esp, 8

        push 5
        push esi
        call [lua_tonumber]
        fstp dword [ebp + ProjDef.targetY]
        add esp, 8

        push 6
        push ebx
        call shok_lua_toint
        mov dword [ebp + ProjDef.damage], eax
        
        push 7
        push esi
        call [lua_tonumber]
        fstp dword [ebp + ProjDef.radius]
        add esp, 8

        push 8
        push ebx
        call shok_lua_toint
        mov dword [ebp + ProjDef.targetID], eax
        
        push 9
        push ebx
        call shok_lua_toint
        mov dword [ebp + ProjDef.attackerID], eax
        
        ; call createeffect
        push ebp
        mov dword ecx, [shok_createprojectileVTP]
        mov dword eax, [ecx]
        
        call [eax+shok_createprojectileVtO] ; createeffect, returns id
        
        ; set 10th arg as callback in global table
        push eax
        
        push sEffectCbsTable
        push ebx
        call shok_lua_pushstring
        
        push LUA_GLOBALSINDEX
        push ebx
        call [lua_rawget]
        add esp, 8
        
        pop eax
        push eax
        
        push eax
        push ebx
        call shok_lua_pushint
        
        push 10
        push ebx
        call [lua_pushvalue]
        add esp, 8
        
        push -3
        push ebx
        call [lua_settable]
        add esp, 8
        
        pop eax
        
        ; set return
        push eax
        push ebx
        call shok_lua_pushint
        
        add esp, ProjDef.size
        
        mov dword [createProjectileMarker], 0
        
        popad
        mov eax, 1
        retn


; bool S5Hook.IsValidEffect(int effectID)
isValidEffect:
        pushad
        mov ebx, [esp + 24h]
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax                        ; effectID
        mov dword ecx, [shok_EffectManagerInstance]       ;EffectManager instance
        call shok_EffectManagerEffectExists                   ;bool Mgr::EffectExists(eID)
        
        movzx eax, al
        push eax
        push ebx
        call [lua_pushboolean]
        
        add esp, 2*4
        
        popad
        mov eax, 1
        retn


getEffectObject:
        pushad
        mov ebx, [esp + 24h]
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax                        ; effectID
        mov dword ecx, [shok_EffectManagerInstance]       ;EffectManager instance
        call shok_EffectManagerEffidToObj                   ;get effect object from id
        
        test eax, eax
        jnz .entityOK
        
        popad
        xor eax, eax
        retn
        
.entityOK:
        
        push eax
        call daCreateUDHead
        
        popad
        mov eax, 1
        retn


doCreatedProjectileCallbackFor: ; lua state in ebx, effid in ecx
        push ecx
        
        ; get table
        push sEffectCbsTable
        push ebx
        call shok_lua_pushstring
        
        push LUA_GLOBALSINDEX
        push ebx
        call [lua_rawget]
        add esp, 8
        
        ; get potential func
        pop ecx
        push ecx
        push ecx
        push ebx
        call shok_lua_pushint
        
        push -2
        push ebx
        call [lua_rawget]
        add esp, 8
        
        ; remove func from global
        pop ecx
        push ecx
        push ecx
        push ebx
        call shok_lua_pushint
        
        push ebx
        call [lua_pushnil]
        add esp, 4
        
        push -4
        push ebx
        call [lua_settable]
        add esp, 8
        
        ; check type
        push -1
        push ebx
        call [lua_type]
        add esp, 8
        
        cmp eax, LUA_TNIL
        je .nocb
        
        ; call function
        pop ecx
        push ecx
        push ecx
        push ebx
        call shok_lua_pushint
        
        push 0                  ; no err func
        push 0                  ; no return values
        push 1                  ; 1 args
        push ebx
        call [lua_pcall]
        add esp, 4*4
        
.nocb:
        pop ecx
        ret

; arrow hit cb: 511336h  /  4FF476h
; vtable adress: 778E24h / 777690h (+24h)

globalHitCallbackArrow:
        pushad
        
        mov ebx, [luaHandle]
        push ecx
        
        
        mov eax, [esp]
        mov ecx, [eax+shok_EGL_CEffectOOEffectID] ; effect id
        call doCreatedProjectileCallbackFor
        
        
        ; global callback
        mov eax, [globalHitCbLuaFunc]
        cmp eax, 0
        je .noCb
        
        
        
        push dword [globalHitCbLuaFunc]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        add esp, 3*4
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectType] ; efftype
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectSX] ; sx
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectSY] ; sy
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectTX] ; tx
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectTY] ; ty
        call luaPushFloat
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectAttID] ; attacker id
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        push dword [eax+shok_GGL_CArrowEffectOOEffectTarID] ; target id
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        push dword [eax+shok_GGL_CArrowEffectOOEffectDamage] ; damage
        push ebx
        call shok_lua_pushint
        
        push -1 ; aoe range
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectID] ; effect id
        push ebx
        call shok_lua_pushint
        
        push 0                  ; no err func
        push 0                  ; no return values
        push 10                 ; 10 args
        push ebx
        call [lua_pcall]
        add esp, 5*4
        
        jmp .return
        
.noCb:
        pop eax
        
.return:
        popad
        push shok_GGL_CArrowEffect_HitCallback ; jump back
        ret

globalHitCallbackCannonBall:
        pushad
        
        mov ebx, [luaHandle]
        push ecx
        
        
        mov eax, [esp]
        mov ecx, [eax+shok_EGL_CEffectOOEffectID] ; effect id
        call doCreatedProjectileCallbackFor
        
        
        ; global callback
        mov eax, [globalHitCbLuaFunc]
        cmp eax, 0
        je .noCb
        
        
        
        push dword [globalHitCbLuaFunc]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        add esp, 3*4
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectType] ; efftype
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectSX] ; sx
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectSY] ; sy
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectTX] ; tx
        call luaPushFloat
        
        mov eax, [esp]
        fld dword [eax+shok_EGL_CEffectOOEffectTY] ; ty
        call luaPushFloat
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectAttID] ; attacker id
        push ebx
        call shok_lua_pushint
        
        push 0 ; target id
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        push dword [eax+shok_GGL_CCannonBallEffectOOEffectDamage] ; damage
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp]
        fld dword [eax+shok_GGL_CCannonBallEffectOOAoERange] ; aoe range
        call luaPushFloat
        
        mov eax, [esp]
        push dword [eax+shok_EGL_CEffectOOEffectID] ; effect id
        push ebx
        call shok_lua_pushint
        
        push 0                  ; no err func
        push 0                  ; no return values
        push 10                 ; 10 args
        push ebx
        call [lua_pcall]
        add esp, 5*4
        
        jmp .return
        
.noCb:
        pop eax
        
.return:
        popad
        push shok_GGL_CCannonBallEffect_HitCallback ; jump back
        ret

setGlobalHitCbHook:
        mov dword [shok_GGL_CArrowEffectVt+shok_EGL_CFlyingEffectVtOHitCallback], globalHitCallbackArrow ; patch vtable
        mov dword [shok_GGL_CCannonBallEffectVt+shok_EGL_CFlyingEffectVtOHitCallback], globalHitCallbackCannonBall
        ret
        

setGlobalHitCb:
        pushad
        call removeGlobalHitCb
        
        push LUA_REGISTRYINDEX ; remember func
        push ebx
        call [luaL_ref]
        mov [globalHitCbLuaFunc], eax
        add esp, 8
        
        popad
        mov eax, 0
        ret

removeGlobalHitCbHook:
        mov dword [shok_GGL_CArrowEffectVt+shok_EGL_CFlyingEffectVtOHitCallback], shok_GGL_CArrowEffect_HitCallback ; unpatch vtable
        mov dword [shok_GGL_CCannonBallEffectVt+shok_EGL_CFlyingEffectVtOHitCallback], shok_GGL_CCannonBallEffect_HitCallback
        ret

removeGlobalHitCb:
        pushad
        mov eax, [globalHitCbLuaFunc]
        test eax, eax
        jz .ghcbret
        
        push eax                    ; free name table reference
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_unref]
        add esp, 3*4
        
        mov dword [globalHitCbLuaFunc], 0
        
.ghcbret:
        popad
        mov eax, 0
        ret

effectCreatedOverride:
        
        push ebx
        push ecx
        mov ebx, [luaHandle]
        
        push 14
        push ebx
        call [lua_checkstack]
        add esp, 2*4
        
        push dword [globalEffectCreatedCbLuaFunc]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        add esp, 3*4
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.effectType]
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.playerID]
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp+0Ch]
        push ebx
        fld dword [eax+ProjDef.startX]
        call luaPushFloat
        add esp, 4
        
        mov eax, [esp+0Ch]
        push ebx
        fld dword [eax+ProjDef.startY]
        call luaPushFloat
        add esp, 4
        
        mov eax, [esp+0Ch]
        push ebx
        fld dword [eax+ProjDef.targetX]
        call luaPushFloat
        add esp, 4
        
        mov eax, [esp+0Ch]
        push ebx
        fld dword [eax+ProjDef.targetY]
        call luaPushFloat
        add esp, 4
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.attackerID]
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.targetID]
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.damage]
        push ebx
        call shok_lua_pushint
        
        mov eax, [esp+0Ch]
        push ebx
        fld dword [eax+ProjDef.radius]
        call luaPushFloat
        add esp, 4
        
        mov eax, [esp+0Ch]
        push dword [eax+ProjDef.creatorType]
        push ebx
        call shok_lua_pushint
        
        pop ecx
        push dword [esp+8]
        
        call [globalEffectCreatedCbOriginal]
        
        push eax
        push eax
        push ebx
        call shok_lua_pushint
        
        push dword [createProjectileMarker]
        push ebx
        call shok_lua_pushint
        
        
        push 0                  ; no err func
        push 0                  ; no return values
        push 13                 ; 13 args
        push ebx
        call [lua_pcall]
        add esp, 4*4
        
        pop eax
        pop ebx
        ret 4

setEffectCreatedCb:
        pushad
        call removeEffectCreatedCb
        
        push LUA_REGISTRYINDEX ; remember func
        push ebx
        call [luaL_ref]
        mov [globalEffectCreatedCbLuaFunc], eax
        add esp, 8
        
        ; patch vtable
        mov dword ecx, [shok_createprojectileVTP]
        mov dword eax, [ecx]
        
        mov ecx, [eax+shok_createprojectileVtO]
        mov dword [globalEffectCreatedCbOriginal], ecx
        mov dword [eax+shok_createprojectileVtO], effectCreatedOverride
        
        popad
        mov eax, 0
        ret


removeEffectCreatedCb:
        pushad
        mov eax, [globalEffectCreatedCbLuaFunc]
        test eax, eax
        jz .ghcbret
        
        push eax                    ; free name table reference
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_unref]
        add esp, 3*4
        
        mov dword [globalEffectCreatedCbLuaFunc], 0 ; unpatch vtable
        
        mov dword ecx, [shok_createprojectileVTP]
        mov dword eax, [ecx]
        
        mov ecx, [globalEffectCreatedCbOriginal]
        mov dword [eax+effectCreatedOverride], ecx
        
        
        
.ghcbret:
        popad
        mov eax, 0
        ret
