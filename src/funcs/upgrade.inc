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
        
        mov ecx, [shok_CGLGameLogicOP]  ; CGLGameLogic
        mov ecx, [ecx+shok_CGLGameLogicOO1]
        push 1              ; playerId
        call shok_CGLGameLogic_getPlayerStatus        ; getCPlayerStatus(this, playerId)
        mov ecx, [eax+shok_playerstatusOOCBuildingUpgradeManager]  ; CBuildingUpgradeManager
        mov edi, ecx
        
        call shok_CBuildingUpgradeManagerAdd        ; CBuildingUpgradeManager::add(this, catId, firstBuilding)
        
        lea ecx, [edi+shok_CBuildingUpgradeManagerOO] 
        push esi
        push esp
        call shok_CBuildingUpgradeManagerGetObj        ; obj = getObj(this (mgr + 32), &catId_);
        mov dword [eax+shok_CBuildingUpgradeManagerGetObjOOScholars], 1  ; num scholars
        mov dword [eax+shok_CBuildingUpgradeManagerGetObjOO0], 0
        pop eax
        
        popad
        xor eax, eax
        retn