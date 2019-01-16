section luaTable
        tableEntry bitAnd,  "BitAnd"
		tableEntry bitOr,	"BitOr"
		tableEntry bitXor,	"BitXor"
		tableEntry bitNot,	"BitNot"


section code


bitAnd:
		pushad
        mov ebx, [esp+24h]
        
        push 2
        push ebx
        call shok_lua_toint		; arg 1
        push eax
        
        push 1
        push ebx
        call shok_lua_toint		; arg 2
        
		pop ecx
		and eax, ecx
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn


bitOr:
		pushad
        mov ebx, [esp+24h]
        
        push 2
        push ebx
        call shok_lua_toint		; arg 1
        push eax
        
        push 1
        push ebx
        call shok_lua_toint		; arg 2
        
		pop ecx
		or eax, ecx
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitXor:
		pushad
        mov ebx, [esp+24h]
        
        push 2
        push ebx
        call shok_lua_toint		; arg 1
        push eax
        
        push 1
        push ebx
        call shok_lua_toint		; arg 2
        
		pop ecx
		xor eax, ecx
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitNot:
		pushad
        mov ebx, [esp+24h]
        
        push 1
        push ebx
        call shok_lua_toint		; arg 1
        
		not eax
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

