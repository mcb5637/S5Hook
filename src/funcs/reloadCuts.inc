section luaTable
	tableEntry reloadCutscenes,	"ReloadCutscenes"
		
section code
reloadCutscenes:
		mov dword eax, [0A0344Ch]
		mov ecx, [eax]
		mov eax, [ecx]
		
		push 779FFCh			; ptr to "Maps\ExternalMap"
		call [eax+0Ch]			; 2nd arg in ecx
		
		xor eax, eax
		retn