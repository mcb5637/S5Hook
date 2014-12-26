section luaTable
	tableEntry reloadCutscenes,	"ReloadCutscenes"
		
section code
reloadCutscenes:

		push 1
		push ebx
		call [lua_tostring]
		add esp, 8
		
		test eax, eax
		jz .defaultDir
		push eax
		jmp .reload
		
.defaultDir:
		push 779FFCh			; ptr to "Maps\ExternalMap"
		
.reload:
		mov dword eax, [0A0344Ch]
		mov ecx, [eax]
		mov eax, [ecx]
	
		call [eax+0Ch]			; 2nd arg in ecx
		
		xor eax, eax
		retn