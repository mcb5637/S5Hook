section luaTable
		tableEntry logToFile,		"Log"

section strings
sLogFormat		db "%s", 0Ah, 0

section code
logToFile:
		push 1
		push ebx
		call [lua_tostring]
		add esp, 8
		
		push eax
		push sLogFormat
		call writeToLog
		add esp, 8
		
		xor eax, eax
		retn