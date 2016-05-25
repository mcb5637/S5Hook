section luaTable
        tableEntry createProjectile,        "CreateProjectile"
        tableEntry isValidEffect,           "IsValidEffect"

section code

struc ProjDef
    .creatorType:   resd 1
    .effectType:    resd 1
                    resd 1
    .playerID       resd 1
    
    .startXcopy:    resd 1
    .startYcopy:    resd 1
    .startX:        resd 1  ;f
    .startY:        resd 1  ;f
    .targetX:       resd 1  ;f
    .targetY:       resd 1  ;f
    
                    resd 1
    .attackerID:    resd 1
    .targetID:      resd 1
    .damage:        resd 1  ;int
    .radius:        resd 1  ;f
    
                    resd 3
    .size:
endstruc



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

              
createProjectile:
        pushad
        mov ebx, [esp + 24h]
        sub esp, ProjDef.size
        mov ebp, esp
        
        xor eax, eax
        mov ecx, ProjDef.size
.clear: 
        mov [ebp + ecx - 1], al
        dec ecx
        jnz .clear
        
        
        mov dword [ebp + ProjDef.creatorType], 774698h        ;CProjectileEffectCreator
        
        push ebx
        call [lua_gettop]
        pop esi
        mov esi, eax
        
        push 1
        call .getLuaFloat
        push 2
        call .getLuaFloat
        push 3
        call .getLuaFloat
        push 4
        call .getLuaFloat
        push 5
        call .getLuaFloat
        
        fstp dword [ebp + ProjDef.targetY]
        fstp dword [ebp + ProjDef.targetX]
        fst dword [ebp + ProjDef.startY]
        fstp dword [ebp + ProjDef.startYcopy]
        fst dword [ebp + ProjDef.startX]
        fstp dword [ebp + ProjDef.startXcopy]
        fistp dword [ebp + ProjDef.effectType]
        
        sub esi, 5
        jz .noDamage
        
        push 6
        call .getLuaFloat
        fistp dword [ebp + ProjDef.damage]
        
        dec esi
        jz .noRadius
        
        push 7
        call .getLuaFloat
        fstp dword [ebp + ProjDef.radius]
        
        dec esi
        jz .noTargetId
        
        push 8
        call .getLuaFloat
        fistp dword [ebp + ProjDef.targetID]
        
        dec esi
        jz .noAttackerId
        
        push 9
        call .getLuaFloat
        fistp dword [ebp + ProjDef.attackerID]
        
        jmp .setupDone
        
.noDamage:
        ;mov dword [ebp + ProjDef.damage], 0        ;already nulled

.noRadius:
        mov dword [ebp + ProjDef.radius], 0BF800000h ; -1.0
        
.noTargetId:
        ;mov dword [ebp + ProjDef.targetID], 0
        
.noAttackerId:
        ;mov dword [ebp + ProjDef.attackerID], 0
        
.setupDone:
        
        push ebp
        mov dword ecx, [895DACh]
        mov dword eax, [ecx]
        
        call [eax+05Ch]
        push eax                    ;effectID
        
        sub esi, 2                  ; if all 10 params, esi becomes 0, no sign
        js .noCb
        
        mov esi, [esp-24h]          ;esi = &effectObj
        
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_ref]
        mov [esi+58h], eax          ; effectObj+58 = playerID (unused for projectiles)
        add esp, 8
        
        push dword [esi]                    ;orig vtable location
        call createFakeEffectVtable
        mov [esi], eax
        
        
.noCb:

        fild dword [esp]            ;effectID
        call luaPushFloat
        pop eax

        add esp, ProjDef.size
        popad
        mov eax, 1
        retn
        
        
.getLuaFloat:
        push dword [esp+4]
        push ebx
        call [lua_tonumber]
        add esp, 8
        retn 4


createFakeEffectVtable:
        push esi
        mov esi, [esp+8]            ;orig vtable
        
        push 4*12
        call malloc
        pop edi
        mov [eax+4*11], esi
        mov edi, eax
        mov ecx, 4*11
        rep movsb
        
        mov dword [eax+24h], arrowHitCallback
        pop esi
        retn 4

arrowHitCallback:
        pushad
        
        mov ebx, [luaHandle]
        push ecx                ; effect obj
        
        push dword [ecx+58h]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        call [luaL_unref]
        
        push 0                  ; no err func
        push 1                  ; 1 return value
        push 0                  ; no args
        push ebx
        call [lua_pcall]
        
        push -1
        push ebx
        call [lua_toboolean]    ; returns true to cancel attack
        mov esi, eax
        
        push 0
        push ebx
        call [lua_settop]
        
        add esp, 11*4
        
        ;cleanup
        pop ecx                 ; effect obj
        mov eax, [ecx]          ; fake vtable
        mov edi, [eax+11*4]     ; 12th element = orig vtable
        mov [ecx], edi
        
        push eax                ; free fake vtable
        call free
        pop eax
        
        test esi, esi
        jnz .stopAttack
        
        popad
        mov eax, [ecx]
        jmp [eax + 24h]        ; original virtual hit method
        
.stopAttack:
        popad
        
        mov eax, [ecx]
        push 1
        call [eax]              ; call destructor on CFlyingEffect
        retn
        
        
; bool S5Hook.IsValidEffect(int effectID)
isValidEffect:
        pushad
        mov ebx, [esp + 24h]
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax                        ; effectID
        mov dword ecx, [0898144h]       ;EffectManager instance
        call 04FAABDh                   ;bool Mgr::EffectExists(eID)
        
        movzx eax, al
        push eax
        push ebx
        call [lua_pushboolean]
        
        add esp, 2*4
        
        popad
        mov eax, 1
        retn