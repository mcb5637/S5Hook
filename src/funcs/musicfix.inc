section luaTable
		tableEntry patchMusicFix,		    "PatchMusicFix"
		tableEntry unpatchMusicFix,	    "UnpatchMusicFix"


section cleanup
		call unpatchMusicFix

section code
unpatchMusicFix:

		; 8B F0 85 F6 74 76

		mov dword [ifpJump], 0F685F08Bh
		mov word [ifpJump+4], 7674h

		xor eax, eax
		retn
		
		
patchMusicFix:

        ; E9 XX XX XX  XX 90

		mov eax, ifpJump
		mov byte [eax], 0E9h			; jmp opcode
		inc eax
		mov dword [eax], ifpOffset		; relative addr of call target
		mov byte [eax+4], 90h           ; nop

		xor eax, eax
		retn

ifpJump		equ 49669Eh
ifpOffset 	equ internalFsPatch - (ifpJump + 5)

ifpRetOK    equ 4966A4h
ifpRetFail  equ 49671Ah

internalFsPatch:
		mov esi, eax            ; overwritten commands
		test esi, esi
		jnz ifpRetOK            ; original stream is OK, continue...
        
        ; stream not found -> lets try internal fs
        
        mov esi, [esp+8]        ; filename
        
        pushad
        sub esp, 300
        mov ebp, esp
        
        mov dword ecx, [88F088h]
		mov eax, [ecx]
		push ebp                ; &filesize
		lea ebx, [ebp+4]
		push ebx                ; &handle
		push esi
		call [eax+14h]          ; creates a file handle
		
		test eax, eax
		jz .failOut             ; file not found in internal fs
		
		push esi                ; filename
		call 40BAB3h            ; get filename extension, ex: ".wav"
		pop ebx
		
		push eax                ; extension
		push dword [ebp]        ; filesize
		push dword [ebp+4]      ; handle
		push 76D600h            ; "\\\\%u,%u.%s"
		push 256                ; max string length
		lea esi, [ebp+8]
		push esi                ; output string
		call 547CB8h            ; snprintf()
		add esp, 24
		
		push 0
		push esi                ; finished string, ex: "\\\\1100,2321.wav"
		mov eax, [859F0Ch]
		push dword [eax+0Ch]    ; this
		call [761488h]          ; AIL_open_stream@12
		
		cmp dword [ebp+4], -1   ; is the handle invalid?, (-1)
		jz .failCloseHandle
		
		add esp, 300
		mov [esp+4], eax        ; write the STREAM into the right slot 
                                ; so it ends up in esi after popad
		popad
		jmp ifpRetOK			; original function
		
.failCloseHandle:
        push eax
        call 54E9ECh            ; close handle
        pop eax
        
.failOut:
        add esp, 300
        popad
        jmp ifpRetFail