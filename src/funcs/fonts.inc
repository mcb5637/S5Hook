section luaTable
        tableEntry getFontConfig,       "GetFontConfig"   ; (int fontID) => (float size, float zOff, float spacing)
        tableEntry setFontConfig,       "SetFontConfig"   ; (int fontID, float size, float zOff, float spacing) => nil
        
section code

getFontObj:
        push 1
        push ebx
        call shok_lua_toint
        push eax
        
        call shok_GetFontIdMgr            ; get font id mgr 
        mov ecx, eax          
        call shok_GetFontObject            ; get font obj(this, id)
        test eax, eax
        retn
        
getFontConfig:
        pushad
        mov ebx, [esp+24h]
        
        call getFontObj
        jz fontNotFound

        fld dword [eax+shok_FontOOSpacing]     ;spacing
        fld dword [eax+shok_FontOOZOffset]       ;zOffset
        fld dword [eax+shok_FontOOSize]       ;size
        
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
        
        fstp dword [eax+shok_FontOOSpacing]     ;spacing
        fstp dword [eax+shok_FontOOZOffset]       ;zOffset
        fstp dword [eax+shok_FontOOSize]       ;size
        
fontNotFound:
        popad
        xor eax, eax
        retn