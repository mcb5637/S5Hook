section luaTable
		tableEntry createProjectile,		"CreateProjectile"

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
;S5Hook.CreateProjectile(12, 3500,4000,3200,2900, 1000, -1, 65541, 131078)
;S5Hook.CreateProjectile(   1   int effectType, 
                        ;   2   float startX,
                        ;   3   float startY, 
                        ;   4   float targetX, 
                        ;   5   float targetY 
                        ;   6   [, int damage=0 
                        ;   7   [, float radius=-1 
                        ;   8   [, int targetId 
                        ;   9   [, int attackerId]]]])

              
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
        
        push eax
        fild dword [esp]
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