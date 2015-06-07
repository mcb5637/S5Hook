section luaTable
		tableEntry patchMusicFix,		    "PatchMusicFix"
		tableEntry unpatchMusicFix,	    "UnpatchMusicFix"

section globalVars
closeStreamOrig		dd 0

section cleanup
		call unpatchMusicFix

AILCloseStream      equ 7614A4h     ; stream
AILSetUserData      equ 761514h     ; stream, idx, value
AILGetUserData      equ 761520h     ; stream, idx

section code
unpatchMusicFix:
        mov eax, [closeStreamOrig]
        test eax, eax
        jz .fail
        
        mov dword [AILCloseStream], eax
        mov dword [closeStreamOrig], 0
		; 8B F0 85 F6 74 76

		mov dword [ifpJump], 0F685F08Bh
		mov word [ifpJump+4], 7674h
		
.fail:
		xor eax, eax
		retn
		
		
patchMusicFix:

        cmp dword [closeStreamOrig], 0
        jnz .fail
        
        mov eax, [AILCloseStream]
        mov [closeStreamOrig], eax
        mov dword [AILCloseStream], closeStreamWrapper
        
        ; E9 XX XX XX  XX 90

		mov eax, ifpJump
		mov byte [eax], 0E9h			; jmp opcode
		inc eax
		mov dword [eax], ifpOffset		; relative addr of call target
		mov byte [eax+4], 90h           ; nop
		
.fail:
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
		mov esi, eax            ; hstream
		
		test esi, esi           ; check the AIL hstream
		jz .failCloseHandle
		
		push dword [ebp+4]
		push 2
		push esi
		call [AILSetUserData]
		
		add esp, 300
		mov [esp+4], esi        ; write the STREAM into the right slot 
                                ; so it ends up in esi after popad
		popad
		jmp ifpRetOK			; original function
		
.failCloseHandle:
        push eax
        call 54E9ECh            ; close fs handle
        pop eax
        
.failOut:
        add esp, 300
        popad
        jmp ifpRetFail


closeStreamWrapper:             ; wrapper for _AIL_close_stream(hstream)
        push 2
        push dword [esp+8]      ; hstream
        call [AILGetUserData]
        test eax, eax
        jz .continueToOrig
        
        push eax
        call 54E9ECh            ; close fs handle
        pop eax
        
.continueToOrig:
        jmp dword [closeStreamOrig]