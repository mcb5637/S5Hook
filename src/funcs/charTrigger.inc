section luaTable
		tableEntry setCharTrig,		"SetCharTrigger"
		tableEntry unpatchCharTrig,	"RemoveCharTrigger"

section globalVars
charTrigRef		dd 0

section cleanup
		call unpatchCharTrig

section code
unpatchCharTrig:
		mov eax, [charTrigRef]
		test eax, eax
		jz .notPatched
		
		push eax 					; free name table reference
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_unref]
		add esp, 12

		mov dword [charCall], charOrigOff
		mov dword [charTrigRef], 0
		
.notPatched:
		xor eax, eax
		retn
		
		
setCharTrig:
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_ref]
		mov [charTrigRef], eax
		add esp, 8
		
		mov eax, charCall				; edit call at this location 
		mov dword [eax], charOffset		; relative addr of call target

		xor eax, eax
		retn

charCall 	equ	40754Dh		; jmp address of call
charOffset 	equ charTrigger - (charCall + 4)
charOrig	equ 54E649h
charOrigOff	equ charOrig - (charCall + 4)

charTrigger:				
		pushad
		mov ebx, [luaHandle]
		
		push dword [charTrigRef]
		push LUA_REGISTRYINDEX
		push ebx
		call [lua_rawgeti]

		fild dword [esp+0Ch]	; keycode
		fstp qword [esp-8]
		sub esp, 8
		push ebx
		call [lua_pushnumber]
		
		push 0					; no err func
		push 0					; no return values
		push 1					; 1 arg
		push ebx
		call [lua_pcall]
		
		push 0
		push ebx
		call [lua_settop]
		
		add esp, 48
		
		popad
		jmp charOrig			; original function
