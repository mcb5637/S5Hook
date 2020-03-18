section luaTable
        tableEntry createEntityIterator,  "EntityIterator"
        tableEntry entityIterateAll,      "EntityIteratorTableize"

section autorun
        call eiSetup

section code

eiPredFuncs:
        tableEntry createCirclePredicate,       "InCircle"          ; x, y, r
        tableEntry createRectPredicate,         "InRect"            ; x0, y0, x1, y1
        tableEntry createPlayerPredicate,       "OfPlayer"          ; pID
        tableEntry createTypePredicate,         "OfType"            ; eTypeID
        tableEntry createCatPredicate,          "OfCategory"        ; entCatID
        tableEntry createUpCatPredicate,        "OfUpgradeCategory" ; upCatID
        tableEntry createBuildingPredicate,     "IsBuilding"        ; -
        tableEntry createSectorPredicate,       "InSector"          ; sectorID
		tableEntry createPlayerNotZeroPredicate,"NotOfPlayer0"		; -
		tableEntry createMultiPlayerPredicate,	"OfAnyPlayer"		; player1, player2, ...
		tableEntry createMultiEtypPredicate,	"OfAnyType"		    ; etyp1, etyp2, ...
		tableEntry createRProviderPredicate,	"ProvidesResource"	; resourceType (raw)
; ideas: luaFnPredicate (prob slower than lua implementation)
        dd 0
        
sPredTbl    db "Predicate", 0

eiSetup:
        push sPredTbl
        push eiPredFuncs
        call registerFuncTable
        
        retn

createEntityIterator:
        pushad
        
        push ebx
        call [lua_gettop]
        pop ecx
        push eax
        inc dword [esp]         ; last arg for pushcclosure, Npreds+1 (for iter UD)
        mov edi, eax
        
        lea eax, [8 + 4*eax]        
        push eax
        push ebx
        call [lua_newuserdata]  ; mem layout |it pos|pred0|pred1|0|
        mov esi, eax
        add esp, 8
        
        mov [esi], dword 0             ; pos = 0
        mov [esi + 4 + 4*edi], dword 0 ; pred list end marker
.nextpred:
        test edi, edi
        jz .done
        
        push edi                    ; push copies of the preds, turn them into upvalues,
        push ebx                    ; to keep them from getting gc'd, but we keep a copy
        call [lua_pushvalue]        ; of the pointers in the iter obj for speed
        call shok_lua_touserdata    ; lua_pushvalue is cdecl, same args
        mov [esi + 4*edi], eax
        
        dec edi
        jmp .nextpred
.done:
        ;n pushed at the top
        push entityIterateLua
        push ebx
        call [lua_pushcclosure] ; (L, fn, Nupvalues)
        add esp, 4*3
        
        mov [esp+24], esi       ;pass obj back in ecx via popad (!), for IterateAll
        popad
        mov eax, 1
        retn


entityIterate:          ; internal iterator, arg = my iter obj
        push esi
        push edi
        push ebp
        mov esi, [esp+10h]      ; iter obj
        mov edi, [shok_globalEntityManager]      ; global entity mgr   
        mov ebp, [edi+shok_globalEntityManagerOOCount]        ; entry count
        add edi, shok_globalEntityManagerOOStartEntityTable            ; start entity table
        lea ebp, [edi + 8*ebp]  ; ebp = end ptr
        add edi, [esi]          ; add current pos, edi = running ptr

.checkEnd:
        cmp edi, ebp
        jge .doneNoRes
        
        mov eax, [edi+4]        ;eax = ptr to entObj    
        test eax, eax
        jz .nextEntity
        
        lea edx, [esi+4]        ; edx = pos of pred ptr
.nextPred:
        mov ecx, [edx]          ; ecx = ptr to predObj
        test ecx, ecx
        jz .doneRes
        
        push edx
        mov edx, [ecx]          ; pred vtable
        push eax
        call [edx+4]            ; predObj.predFn(entObj)
        test al, al             ; bool result
        mov eax, [esp-4]        ; "recover" the entObj ptr
        pop edx
        jz .nextEntity
        
        add edx, 4
        jmp .nextPred
        
        
.nextEntity:
        add edi, 8
        jmp .checkEnd
        
.doneNoRes:
        xor eax, eax
        jmp .writeBack
.doneRes:
        mov eax, [edi+4]        ; entityObj
        mov eax, [eax+shok_EGL_CGLEEntityOO_Id]        ; eID
        
.writeBack:
        sub edi, 6*4-8
        sub edi, [shok_globalEntityManager]
        mov [esi], edi          ; write back current pos
        
        pop ebp
        pop edi
        pop esi
        retn 4


entityIterateLua:
        pushad
        
        push lua_upvalueindex(1)
        push ebx
        call shok_lua_touserdata
        
        push eax
        call entityIterate
        test eax, eax
        jz .noEntity
        
        push eax
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax, 1
        retn
        
.noEntity:
        popad
        xor eax, eax
        retn

        
entityIterateAll: ; Q'n'D
        pushad
        
        call createEntityIterator
        mov esi, ecx                ; hacky calling convention, iter obj
        mov edi, 1                  ; counter for tbl
        
        push ebx
        call [lua_newtable]
        add esp, 4

.iterate:
        push esi
        call entityIterate
        test eax, eax
        jz .done
        
        push eax
        push ebx
        call shok_lua_pushint
        
        push edi    ;n
        push -2     ;tbl stack pos
        push ebx
        call [lua_rawseti]
        add esp, 4*3
        
        inc edi
        jmp .iterate
        
.done:
        popad
        mov eax, 1
        retn

;Predicates        
createCirclePredicate:
        pushad
        
        push 1
        call lua_getfloat
        push 2
        call lua_getfloat
        push 3
        call lua_getfloat
        
        sub esp, 3*4
        mov ebp, esp
        
        fstp dword [ebp]
        fstp dword [ebp+8]
        fstp dword [ebp+4]
        
        push 10h
        push ebx
        call [lua_newuserdata]
        mov ecx, eax
        add esp, 8
        
        push dword [ebp]
        lea eax, [ebp+4]
        push eax
        call shok_CGLEEntityDistancePredicate_ctor_579F9A ; thiscall CGLEEntityDistancePredicate_ctor_579F9A(<ecx> this, float centerPosXY[], float r)
        
        add esp, 3*4
        popad
        mov eax, 1
        retn

createRectPredicate:
        pushad
        
        push 1              ;x0     st(3)
        call lua_getfloat
        push 3              ;x1     st(2)
        call lua_getfloat
        push 2              ;y0     st(1)
        call lua_getfloat
        push 4              ;y1     st(0)
        call lua_getfloat
        
        sub esp, 4*4
        mov ebp, esp
        
        ; EBP + 0  X lo
        ;     + 4  Y lo
        ;     + 8  X hi
        ;     +12  Y hi
        
        fcomi st0, st1              ; CF = st(0) < st(1)
        jb .storeY                  ; jmp on CF=1
        fxch st0, st1
.storeY:
        fstp dword [ebp+4]
        fstp dword [ebp+12]
        
        fcomi st0, st1              ; CF = st(0) < st(1)
        jb .storeX                  ; jmp on CF=1
        fxch st0, st1
.storeX:
        fstp dword [ebp]
        fstp dword [ebp+8]   
        
        push 14h
        push ebx
        call [lua_newuserdata]
        mov ecx, eax
        add esp, 8
        
        push ebp
        add dword [esp], 8
        push ebp
        call shok_CGLEEntityPredicateArea_ctor_579EF8 ; thiscall CGLEEntityPredicateArea_ctor_579EF8(<ecx> this, float posA[], float posB[])
        
        add esp, 4*4
        popad
        mov eax, 1
        retn

createPlayerPredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CPlayerPredicate    ;CPlayerPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax
        
        popad
        mov eax, 1
        retn

createTypePredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CGLEEntityTypePredicate    ;CGLEEntityTypePredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax
        
        popad
        mov eax, 1
        retn
        
createBuildingPredicate:
        pushad
        
        push 4
        push ebx
        call [lua_newuserdata]
        add esp, 8
        
        mov [eax], dword shok_CGLIsBuildingPredicate    ;CGLIsBuildingPredicate
        
        popad
        mov eax, 1
        retn

createCatPredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CEntityCategoryPredicate    ;CEntityCategoryPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; catID
        
        popad
        mov eax, 1
        retn

createUpCatPredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CUpgradeCategoryPredicate    ;CUpgradeCategoryPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; upCatID
        
        popad
        mov eax, 1
        retn
        
        
createRProviderPredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CResourceProviderPredicate    ;CResourceProviderPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; resource type
        
        popad
        mov eax, 1
        retn
     
createSectorPredicate:
        pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], dword shok_CSectorPredicate    ;CSectorPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; sectorID
        
        popad
        mov eax, 1
        retn

createPlayerNotZeroPredicate:
		pushad
        
        push 8
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
        mov [esi], esi									; pointer to own adress -> object is own vtable
		
		mov [esi+4], dword playerNotZeroPredicateFunc	; func at +4
        
        popad
        mov eax, 1
        retn

playerNotZeroPredicateFunc:		; ecx -> arg0 this (not used)
		pushad
		
		mov ebx, [esp+24h] ; arg1 -> pointer to entity object
		
		mov ecx, [ebx+shok_EGL_CGLEEntityOO_PlayerId] ; load player
		
		cmp ecx, 0 ; check zero
		jne .rtrue
		
		
		popad
		mov eax, 0
		retn 4
		
.rtrue:
		popad
		mov eax, 1
		retn 4

createMultiPlayerPredicate:
		pushad
		
		push ebx
		call [lua_gettop]
		pop ebx
		mov ecx, eax						; number of args
		push ecx
		
		imul eax, eax, 4
		add eax, 12					; size of userdata: (vtable, func, size, arg1, arg2...)
		
        push eax				; create object
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
		pop ecx
		
        mov [esi], esi									; pointer to own adress -> object is own vtable
		
		mov [esi+4], dword multiPlayerPredicateFunc	; func at +4
        
		mov [esi+8], ecx							; number of args
		
		mov edx, 0								; arg counter
		
.loop:
		cmp edx, ecx						; copy players
		je .loopEnd
		
		add edx, 1
		
		push edx
		push ecx
		
		push edx
		push ebx
		call shok_lua_toint
		
		pop ecx
		pop edx
		
		mov [esi+8+edx*4], eax
		
		jmp .loop
		
.loopEnd:
        popad
        mov eax, 1
        retn

multiPlayerPredicateFunc:		; ecx -> arg0 this
		pushad
		
		mov ebx, [esp+24h] ; arg1 -> pointer to entity object
		
		mov ebx, [ebx+shok_EGL_CGLEEntityOO_PlayerId] ; load player
		
		mov eax, 0
		
.loopStart:
		mov edx, [ecx+8] ; number of players
		
		cmp edx, eax	; loop end
		je .rfalse
		
		mov edx, [ecx+12+eax*4]	; compare player
		cmp ebx, edx
		je .rtrue
		
		add eax, 1
		jmp .loopStart
		
.rfalse:
		popad
		mov eax, 0
		retn 4
		
.rtrue:
		popad
		mov eax, 1
		retn 4

createMultiEtypPredicate:
		pushad
		
		push ebx
		call [lua_gettop]
		pop ebx
		mov ecx, eax						; number of args
		push ecx
		
		imul eax, eax, 4
		add eax, 12					; size of userdata: (vtable, func, size, arg1, arg2...)
		
        push eax					; create object
        push ebx
        call [lua_newuserdata]
        mov esi, eax
        add esp, 8
        
		pop ecx
		
        mov [esi], esi									; pointer to own adress -> object is own vtable
		
		mov [esi+4], dword multiEtypPredicateFunc	; func at +4
        
		mov [esi+8], ecx							; number of args
		
		mov edx, 0								; arg counter
		
.loop:
		cmp edx, ecx						; copy etypes
		je .loopEnd
		
		add edx, 1
		
		push edx
		push ecx
		
		push edx
		push ebx
		call shok_lua_toint
		
		pop ecx
		pop edx
		
		mov [esi+8+edx*4], eax
		
		jmp .loop
		
.loopEnd:
        popad
        mov eax, 1
        retn

multiEtypPredicateFunc: ; ecx -> arg0 this
		pushad
		
		mov ebx, [esp+24h] ; arg1 -> pointer to entity object
		
		mov ebx, [ebx+shok_EGL_CGLEEntityOO_TypeId] ; load etyp
		
		mov eax, 0
		
.loopStart:
		mov edx, [ecx+8] ; number of etypes
		
		cmp edx, eax	; loop end
		je .rfalse
		
		mov edx, [ecx+12+eax*4]	; compare etyp
		cmp ebx, edx
		je .rtrue
		
		add eax, 1
		jmp .loopStart
		
.rfalse:
		popad
		mov eax, 0
		retn 4
		
.rtrue:
		popad
		mov eax, 1
		retn 4


