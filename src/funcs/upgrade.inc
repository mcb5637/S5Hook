section luaTable
        tableEntry addBuildingUpgrade,      "AddBuildingUpgrade"    ; (upCatID, firstBuildingTypeID)

section code
addBuildingUpgrade:
        pushad
        
        push 2              ; firstBuildingId
        push ebx
        call shok_lua_toint
        push eax
        
        push 1              ; catId
        push ebx
        call shok_lua_toint
        push eax
        mov esi, eax
        
        ;ref: 4BC18D & 4CA751
        
        mov ecx, [85A3A0h]  ; CGLGameLogic
        mov ecx, [ecx+40]
        push 1              ; playerId
        call 4A91BCh        ; getCPlayerStatus(this, playerId)
        mov ecx, [eax+792]  ; CBuildingUpgradeManager
        mov edi, ecx
        
        call 4B3D42h        ; CBuildingUpgradeManager::add(this, catId, firstBuilding)
        
        lea ecx, [edi+20h] 
        push esi
        push esp
        call 4CA5A2h        ; obj = getObj(this (mgr + 32), &catId_);
        mov dword [eax], 1  ; num scholars
        mov dword [eax+4], 0
        pop eax
        
        popad
        xor eax, eax
        retn