section luaTable
		tableEntry setSMotivation,		"SetSettlerMotivation"

section code
setSMotivation:
		pushad
		
		push 1
		push ebx
		call shok_lua_toint
		
		push eax
		call shok_eid2obj
		test eax, eax
		jz .noentity
		
		lea ecx, [eax+0B4h]			;this
		
		mov ebx, 13007h				
		push ebx
		push ebx
		mov ebx, esp
		push ebx
		add ebx, 4
		push ebx
		call 58403Eh
		
		mov edi, [ecx+10h]
		mov edi, [edi+4]
		
		push 2
		push esi
		call [lua_tonumber]
		fstp dword [edi+18h]
		
		add esp, 10h
		
.noentity:
		popad
		xor eax, eax
		retn