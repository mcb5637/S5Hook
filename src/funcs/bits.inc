section luaTable
        tableEntry bitAnd,  "BitAnd"
		tableEntry bitOr,	"BitOr"
		tableEntry bitXor,	"BitXor"
		tableEntry bitNot,	"BitNot"
		tableEntry bitAShR,	"BitAShR"
		tableEntry bitShR,	"BitShR"
		tableEntry bitShL,	"BitShL"
		tableEntry bitRoR,	"BitRoR"
		tableEntry bitRoL,	"BitRoL"


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

bitAShR:
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
        sar eax, cl
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitShR:
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
        shr eax, cl
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitShL:
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
        shl eax, cl
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitRoR:
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
        ror eax, cl
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

bitRoL:
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
        rol eax, cl
		
        push eax
        push ebx
        call shok_lua_pushint 	; return stuff
        popad
        mov eax, 1
        retn

