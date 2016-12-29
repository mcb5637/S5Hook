section luaTable
        tableEntry createEntityIterator,  "EntityIterator"

section autorun
        call eiSetup

section code

eiPredFuncs:
        tableEntry createCirclePredicate,       "InCircle"          ; x, y, r
;       tableEntry createRectPredicate,         "InRect"            ; x0, y0, x1, y1
        tableEntry createPlayerPredicate,       "OfPlayer"          ; pID
        tableEntry createTypePredicate,         "OfType"            ; eTypeID
        tableEntry createCatPredicate,          "OfCategory"        ; entCatID
        tableEntry createUpCatPredicate,        "OfUpgradeCategory" ; upCatID
        tableEntry createBuildingPredicate,     "IsBuilding"        ; -
        tableEntry createSectorPredicate,       "InSector"          ; sectorID
;       tableEntry createLuaPredicate,          "Lua"               ; luaFn
; , resourceProvider, sector
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
        
        popad
        mov eax, 1
        retn



entityIterate:          ; internal iterator, arg = my iter obj
        push esi
        push edi
        push ebp
        mov esi, [esp+10h]      ; iter obj
        mov edi, [897558h]      ; global entity mgr   
        mov ebp, [edi+4]        ; entry count
        add edi, 6*4            ; start entity table
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
        mov eax, [eax+8]        ; eID
        
.writeBack:
        sub edi, 6*4-8
        sub edi, [897558h]
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
        call 579F9Ah ; thiscall CGLEEntityDistancePredicate_ctor_579F9A(<ecx> this, float centerPosXY[], float r)
        
        add esp, 3*4
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
        
        mov [esi], dword 77B900h    ;CPlayerPredicate
        
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
        
        mov [esi], dword 766CA0h    ;CGLEEntityTypePredicate
        
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
        
        mov [eax], dword 77046Ch    ;CGLIsBuildingPredicate
        
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
        
        mov [esi], dword 774E84h    ;CEntityCategoryPredicate
        
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
        
        mov [esi], dword 777964h    ;CUpgradeCategoryPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; upCatID
        
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
        
        mov [esi], dword 772B24h    ;CSectorPredicate
        
        push 1
        push ebx
        call shok_lua_toint
        mov [esi+4], eax            ; sectorID
        
        popad
        mov eax, 1
        retn