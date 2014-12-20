section luaTable
		tableEntry reloadEntities, 	"ReloadEntities"
;		tableEntry resetLoadedFlag, "ResetLoadedFlag"

section code
reloadEntities:
		mov eax, [84EF60h]		; [[[84ef60]+258]+0C]
		mov eax, [eax+258h]
		mov eax, [eax+0Ch]
		push eax
		call 5B9A97h
		
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