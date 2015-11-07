bits 32
section payload
%include 'globals.inc'

shiftedOrigin 	equ	hookBase - ( startOfPayload - payloadHeader)
org shiftedOrigin


payloadHeader:							; for stage1 loader

entryPoint		dd installer
copySize		dd payloadSize

startOfPayload equ $

	; const strings
section strings align=1
sS5Hook			db "S5Hook", 0

section luaTable align=1
luaFuncTable:
				tableEntry triggerInt3,		"Break"
				; dd 0 ; omit because of cnTableRef ;)

section globalVars align=1

section code align=1
installer:
		pushad 
		
		mov esi, luaFuncTable
		
.nextEntry:
		push 0						; no description
		push dword [esi]			; func ptr
		add esi, 5
		push esi					; func name (string)
		movzx eax, byte [esi-1]		; skip over func name
		add esi, eax
		push sS5Hook				; base table
		push ebx					; lua handle
		call regLuaFunc
		
		cmp dword [esi], 0
		jnz .nextEntry
		
		; patch functions to unload s5hook
		mov eax, leaveJump				; create jump at this location 
		mov byte [eax], 0E9h			; opcode jmp
		mov dword [eax+1], leaveOffset	; relative jmp target
		
		mov eax, loadJump				; create jump at this location 
		mov byte [eax], 0E9h			; opcode jmp
		mov dword [eax+1], loadOffset	; relative jmp target
		
		popad
		retn
		
%include 'funcs/musicfix.inc'
%include 'funcs/osi.inc'
%include 'funcs/runtimeStore.inc'
%include 'funcs/changeString.inc'
%include 'funcs/log.inc'
%include 'funcs/addArchive.inc'
%include 'funcs/reloadCuts.inc'
%include 'funcs/loadGUI.inc'
%include 'funcs/evalLua.inc'
%include 'funcs/customNames.inc'
%include 'funcs/charTrigger.inc'
%include 'funcs/keyTrigger.inc'
%include 'funcs/mouseTrigger.inc'
%include 'funcs/motivation.inc'
%include 'funcs/reloadConfig.inc'
%include 'funcs/widget.inc'
%include 'funcs/projectile.inc'

	
leaveJump 	equ	40AA1Fh
leaveOffset equ leaveGameHook - (leaveJump + 5)
leaveGameHook:				; jmp from 40AA1F
		lea edi, [esi+2A4h]	; overwritten instruction
		pushad
		
		mov ebx, [luaHandle]
		call unpatchEverything
		
		popad
		jmp 40AA25h


loadJump 	equ	40AA76h
loadOffset 	equ loadGameHook - (loadJump + 5)
loadGameHook:				; jmp from 40AA76
		mov eax, 72A2C0h	; overwritten instruction
		pushad
		
		mov ebx, [luaHandle]
		call unpatchEverything
		
		popad
		jmp 40AA7Bh
		
triggerInt3:
		int3
		xor eax, eax
		retn

unpatchEverything:
section cleanup align=1
		retn

payloadSize equ $ - hookBase