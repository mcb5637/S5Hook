section luaTable
		tableEntry setKeyTrig,		"SetKeyTrigger"
		tableEntry unpatchKeyTrig,	"RemoveKeyTrigger"
		
section globalVars
keyTrigRef		dd 0

section cleanup
		call unpatchKeyTrig

section code
unpatchKeyTrig:
		mov eax, [keyTrigRef]
		test eax, eax
		jz .notPatched
		
		push eax 					; free name table reference
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_unref]
		add esp, 12

		mov dword [keyCall], keyOrigOff
		mov dword [keyTrigRef], 0
		
.notPatched:
		xor eax, eax
		retn
		
		
setKeyTrig:
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_ref]
		mov [keyTrigRef], eax
		add esp, 8
		
		mov eax, keyCall				; edit call at this location 
		mov dword [eax], keyOffset		; relative addr of call target

		xor eax, eax
		retn

keyCall 	equ	40757Eh		; jmp address of call
keyOffset equ keyTrigger - (keyCall + 4)
keyOrig		equ 54E82Fh
keyOrigOff	equ keyOrig - (keyCall + 4)

keyTrigger:				; call from 40757E
		pushad
		mov edi, ecx			; save event type
		mov ebx, [luaHandle]
		
		push dword [keyTrigRef]
		push LUA_REGISTRYINDEX
		push ebx
		call [lua_rawgeti]

		fild dword [esp+0Ch]	; keycode
		fstp qword [esp-8]
		sub esp, 8
		push ebx
		call [lua_pushnumber]
		
		mov eax, edi			; 100&104 -> down, 101&105 -> up
		mov cl, 2
		div cl					; int divide by 2
		mov al, ah				; copy remainder over result
		push eax				; 0101 -> true, 0000 -> false
		push ebx
		call [lua_pushboolean]
		
		push 0					; no err func
		push 0					; no return values
		push 2					; 2 args
		push ebx
		call [lua_pcall]
		
		push 0
		push ebx
		call [lua_settop]
		
		add esp, 56
		
		popad
		jmp keyOrig			; original function