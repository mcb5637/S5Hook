section luaTable
        tableEntry getWidgetPosition,       "GetWidgetPosition"
        tableEntry getWidgetSize,           "GetWidgetSize"
        
section autorun
        call widgetPatchSetPAS
        
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

widgetPatchOneLoadFn:
        add dword [eax+1], (59BD99h-59BD81h)
        mov word [eax+5], 04EBh
        retn
        
widgetPatchSetPAS:
        mov eax, 5617A7h
        cmp byte [eax+5], 0EBh
        je .done
        
        call widgetPatchOneLoadFn
        add eax, (5617B8h - 5617A7h)
        call widgetPatchOneLoadFn
        add eax, (5617C9h - 5617B8h)
        call widgetPatchOneLoadFn
        add eax, (5617DAh - 5617C9h)
        call widgetPatchOneLoadFn
        mov word [eax+11], 0F685h           ;test esi, esi
.done:
        retn