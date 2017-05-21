section luaTable
        tableEntry getFontConfig,       "GetFontConfig"   ; (int fontID) => (float size, float zOff, float spacing)
        tableEntry setFontConfig,       "SetFontConfig"   ; (int fontID, float size, float zOff, float spacing) => nil
        
section code

getFontObj:
        push 1
        push ebx
        call shok_lua_toint
        push eax
        
        call 5593ADh            ; get font id mgr 
        mov ecx, eax          
        call 5597C7h            ; get font obj(this, id)
        test eax, eax
        retn
        
getFontConfig:
        pushad
        mov ebx, [esp+24h]
        
        call getFontObj
        jz fontNotFound

        fld dword [eax+0Ch]     ;spacing
        fld dword [eax+8]       ;zOffset
        fld dword [eax+4]       ;size
        
        call luaPushFloat
        call luaPushFloat
        call luaPushFloat
        
        popad
        mov eax, 3
        retn
        
setFontConfig:
        pushad
        mov ebx, [esp+24h]
        
        push 2
        push ebx
        call shok_lua_tonumber
        
        push 3
        push ebx
        call shok_lua_tonumber
        
        push 4
        push ebx
        call shok_lua_tonumber
        
        call getFontObj
        jz fontNotFound
        
        fstp dword [eax+0Ch]     ;spacing
        fstp dword [eax+8]       ;zOffset
        fstp dword [eax+4]       ;size
        
fontNotFound:
        popad
        xor eax, eax
        retn