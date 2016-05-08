%include 'globals.inc'

bits 32
section text
org loaderBase

loader:
		mov dword [ecx], 76E3CCh ; restore entity obj
		push 0
		call 57A65Eh			 ; call original DestroyEntity()
		pushad
		
		mov al, [hookRsrcFlag]
		test al, al
		jnz .copyPayload
		
        
        ; cache cursor ?    
        
		push bufferOffset		; get write permissions for .text, .data, and .rsrc
		push 40h				; new access: R/W/X
		push 64B000h			; length
		push 401000h			; start of segment 
		call [virtualProtect]
		test eax, eax
		jz .abort
		
		call [lua_open]
		mov [rtState], eax
		
		mov byte [hookRsrcFlag], 1

.copyPayload:	
		
		push 2
		push ebx
		call [lua_tostring]
		pop ecx
		pop edi
		
	    mov ecx, [eax+4]		; num bytes
	    mov edi, hookBase
	    lea esi, [eax+8]
		rep movsb
		
		call [eax]				; run payload
		
.abort:
        popad
		retn 4

bufferOffset:

;padding equ (4 - (($-$$) % 4)) % 4
;times padding int3				; pad to n*4bytes