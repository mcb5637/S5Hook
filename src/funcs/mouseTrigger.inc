section luaTable
		tableEntry setMouseDownTrig,		"SetMouseDownTrigger"
		tableEntry unpatchMouseDownTrig,	"RemoveMouseDownTrigger"

section globalVars
mouseDownTrigRef		dd 0

section cleanup
		call unpatchMouseDownTrig

section code
unpatchMouseDownTrig:
		mov eax, [mouseDownTrigRef]
		test eax, eax
		jz .notPatched
		
		push eax
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_unref]
		add esp, 12
		
		;CPU Dump 0055A58E  C7 46 04 02|00 00 00 EB| 
		mov dword [mdJump], 020446C7h
		mov dword [mdJump+4], 0EB000000h
		
		mov dword [mouseDownTrigRef], 0

.notPatched:
		xor eax, eax
		retn
		
		
setMouseDownTrig:
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_ref]
		mov [mouseDownTrigRef], eax
		add esp, 8
		
		mov eax, mdJump
		mov byte [eax], 0E9h			; jmp opcode
		inc eax
		mov dword [eax], mdOffset		; relative addr of call target

		xor eax, eax
		retn

mdJump		equ 55A58Eh
mdReturn 	equ 55A595h
mdOffset 	equ mouseDownTrigger - (mdJump + 5)

mouseDownTrigger:
		mov dword [esi+4], 2	; original instruction
		
		pushad
		mov ebx, [luaHandle]
		
		push dword [mouseDownTrigRef]
		push LUA_REGISTRYINDEX
		push ebx
		call [lua_rawgeti]
		
		mov eax, [ebp+10h]
		fild dword [eax+10h]	; widget id
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
		jmp mdReturn			; original function
