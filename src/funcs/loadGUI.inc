section luaTable
		tableEntry loadGuiXml,		"LoadGUI"

section code
loadGuiXml:
		pushad
		
		push 0					; arg1
		
		push 1
		push ebx
		call [lua_tostring]
		add esp, 8
		push eax				; arg2 = path
		
		call 5563CCh
		mov ecx, eax			; arg3 in ecx
		call 556464h
		
		popad
		xor eax, eax
		retn