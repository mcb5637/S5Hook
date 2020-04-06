section luaTable
        tableEntry reloadEntities,  "ReloadEntities"
;       tableEntry resetLoadedFlag, "ResetLoadedFlag"

section code
reloadEntities:
        mov eax, [shok_ReloadEntitiesPtr]      ; [[[84ef60]+258]+0C]
        mov eax, [eax+shok_ReloadEntitiesO1]
        mov eax, [eax+shok_ReloadEntitiesO2]
        push eax
        call shok_ReloadEntitiesFunc
        
        xor eax, eax
        retn

%if 0
resetLoadedFlag:
        ;mov eax, [84EF60h]
        ;mov byte [eax+254h], 0
        mov word [40A836h], 9090h
        
        xor eax, eax
        retn
%endif