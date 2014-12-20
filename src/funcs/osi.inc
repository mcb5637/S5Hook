section luaTable
		tableEntry osiLoadImg,		"OSILoadImage"
		tableEntry osiGetImgSz,		"OSIGetImageSize"
		tableEntry osiDrawImg, 		"OSIDrawImage"
		tableEntry osiDrawText, 	"OSIDrawText"
		tableEntry osiSetDrawTrig,	"OSISetDrawTrigger"
		tableEntry osiUnpatchDrawTrig, "OSIRemoveDrawTrigger"
		
section globalVars
osiDrawTrigRef		dd 0

section cleanup
		call osiUnpatchDrawTrig
		
section code
osiLoadImg: 					; img <- fn(path)
		mov esi, 0FFh
		push esi
		push esi
		push esi
		push esi
		sub esp, 16
		fld1
		fstp dword [esp+12]
		fld1
		fstp dword [esp+8]
		fldz
		fstp dword [esp+4]
		fldz
		fstp dword [esp]
		
		push 1
		push ebx
		call [lua_tostring]
		add esp, 8
		push eax
		
		push 40
		call malloc
		pop ecx
		mov ecx, eax
		
		call 55A978h
		push eax
		push ebx
		call [lua_pushlightuserdata]
		add esp, 8
		
		mov esi, ebx
		
		xor eax, eax
		inc eax
		retn

osiGetImgSz: 					; (sx, sy) <- fn(img)
		sub esp, 8				;space for 2x float
		mov ebp, esp
		
		push 1
		push ebx
		call [lua_touserdata]
		add esp, 8		;clean
		
		mov ecx, eax 			; 1st arg for thiscall
		
	    push ebp
	    add ebp, 4
		push ebp
		
		call 55AA52h
		
		fld dword [ebp-4]
		fld dword [ebp]
		call luaPushFloat
		call luaPushFloat
		
		add esp, 8		
		
		mov eax, 2
		retn
		
		
osiDrawImg:					; fn(img, px, py, sx, sy)
		sub esp, 16			; space for rect struct
		mov ebp, esp
		
		push 2 				;posx
		push ebx
		call [lua_tonumber]
		push 3
		push ebx
		call [lua_tonumber]
		push 4				;sizex
		push ebx
		call [lua_tonumber]
		push 5
		push ebx
		call [lua_tonumber]
		
		fstp dword [ebp+12]
		fstp dword [ebp+8]
		fstp dword [ebp+4]
		fstp dword [ebp]
		
		push ebp			; arg4: ptr to rect struct
		push 0				; arg3: ?
		
		push 1
		push ebx
		call [lua_touserdata]
		add esp, 8
		push eax			; arg2: img obj ptr
		
		call 556F5Bh
		mov ecx, eax		; arg1: (thiscall) draw surface?
		call 557167h		; draw
		
		add esp, 48
		
		xor eax, eax
		retn
		
osiDrawText: 				; fn(text, font, x, y, r, g, b, a)
							;	 1s    2i    3f 4f 5i 6i 7i 8i
		sub esp, 16			; space for color struct
		mov ebp, esp
		
		push 2
		push ebx
.nextNum:
		call [lua_tonumber]
		inc dword [esp+4]
		cmp dword [esp+4], 9
		jne .nextNum
		add esp, 8
		fistp dword [ebp+12]
		fistp dword [ebp+8]
		fistp dword [ebp+4]
		fistp dword [ebp]
		
		push 0				; arg10: 0
		push 0				; arg9: 0
		push ebp			; arg8: ptr to color struct
		push 0				; arg7: 0
		push eax ; dummy
		fstp dword [esp]	; arg6: y
		push eax ; dummy
		fstp dword [esp]	; arg5: x
		push 0				; arg4: 0
		push eax
		fistp dword [esp]	; arg3: font type
		
		push 1
		push ebx
		call [lua_tostring]
		add esp, 8
		push eax			; arg2: string
		
		call 556F5Bh
		mov ecx, eax		; arg1: (thiscall) draw surface?
		call 5577E1h		; draw
		
		add esp, 16
		
		xor eax, eax
		retn		
		
drawJump		equ 54167Bh
drawReturn 		equ 541680h
drawOffset 		equ osiDrawTrigger - (drawJump + 5)

osiSetDrawTrig:
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_ref]
		mov [osiDrawTrigRef], eax
		add esp, 8
		
		mov eax, drawJump
		mov byte [eax], 0E9h			; jmp opcode
		inc eax
		mov dword [eax], drawOffset		; relative addr of call target

		xor eax, eax
		retn
		
		
osiUnpatchDrawTrig:
		mov eax, [osiDrawTrigRef]
		test eax, eax
		jz .notPatched
		
		push eax
		push LUA_REGISTRYINDEX
		push ebx
		call [luaL_unref]
		add esp, 12
		
		mov dword [drawJump], 56EC8B55h
		mov dword [drawJump+4], 0C7D8B57h
		
		mov dword [osiDrawTrigRef], 0

.notPatched:
		xor eax, eax
		retn


osiDrawTrigger:		
		; original instructions:
		push ebp
		mov ebp, esp
		push esi
		push edi
		; end
		
		pushad
		
		mov ebx, [luaHandle]
		
		push dword [osiDrawTrigRef]
		push LUA_REGISTRYINDEX
		push ebx
		call [lua_rawgeti]
		
		mov eax, [ebp+0Ch]		; ptr to draw info struct
		fild dword [eax+44h]	; eID
		call luaPushFloat
		
		xor eax, eax
		cmp byte [ebp+207h], al ; active / mouseover flag
		setnz al
		push eax
		push ebx
		call [lua_pushboolean]
		
		mov eax, [ebp+8]
		fld dword [eax+4]
		fld dword [eax]
		call luaPushFloat
		call luaPushFloat
		
		push 0					; no err func
		push 0					; no return values
		push 4					; 4 args (eID, active?, x, y)
		push ebx
		call [lua_pcall]
		
		push 0
		push ebx
		call [lua_settop]
		
		add esp, 44
		
		popad
		jmp drawReturn			; original function