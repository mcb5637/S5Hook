section luaTable
        tableEntry getWidgetPosition,       "GetWidgetPosition"
        tableEntry getWidgetSize,           "GetWidgetSize"

section code
getWidgetObject:
        push 1
        push ebx
        call 55AE12h  ; widget in eax

        push eax
        call 558473h
        mov ecx, eax
        call 558966h ; widget obj in eax
        
        add esp, 8
        retn

luaPushFloat:
        fstp qword [esp-8]
        sub esp, 8
        push ebx
        call [lua_pushnumber]
        
        add esp, 12
        retn

getWidgetPosition:
        pushad
        mov ebx, [esp+24h]
        
        call getWidgetObject
        test eax, eax
        jz widgetNotFound

        fld dword [eax+18h]   ;y pos
        fld dword [eax+14h]   ;x pos
        
        call luaPushFloat
        call luaPushFloat
        
        popad
        mov eax, 2
        retn
        
getWidgetSize:
        pushad
        mov ebx, [esp+24h]
        
        call getWidgetObject
        test eax, eax
        jz widgetNotFound

        fld dword [eax+20h]   ;y size
        fld dword [eax+1Ch]   ;x size
        
        call luaPushFloat
        call luaPushFloat
        
        popad
        mov eax, 2
        retn

widgetNotFound:
        popad
        xor eax, eax
        retn