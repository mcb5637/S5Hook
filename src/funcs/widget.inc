section luaTable
        tableEntry getWidgetPosition,       "GetWidgetPosition"
        tableEntry getWidgetSize,           "GetWidgetSize"
        
section autorun
        call widgetPatchSetPAS
        
section code
getWidgetObject:
        push 1
        push ebx
        call shok_GetWidgetObjF1  ; widget in eax

        push eax
        call shok_GetWidgetObjF2
        mov ecx, eax
        call shok_GetWidgetObjF3 ; widget obj in eax
        
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

        fld dword [eax+shok_widgetOOy]   ;y pos
        fld dword [eax+shok_widgetOOx]   ;x pos
        
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

        fld dword [eax+shok_widgetOOySiz]   ;y size
        fld dword [eax+shok_widgetOOxSiz]   ;x size
        
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
        add dword [eax+shok_widgetpatchOff1], (shok_widgetpatch5-shok_widgetpatch6)
        mov word [eax+shok_widgetpatchOff2], shok_widgetpatchOff2Cmp
        retn
        
widgetPatchSetPAS:
        mov eax, shok_widgetpatch1
        cmp byte [eax+shok_widgetpatchOff3], shok_widgetpatchOff3Cmp
        je .done
        
        call widgetPatchOneLoadFn
        add eax, (shok_widgetpatch2 - shok_widgetpatch1)
        call widgetPatchOneLoadFn
        add eax, (shok_widgetpatch3 - shok_widgetpatch2)
        call widgetPatchOneLoadFn
        add eax, (shok_widgetpatch4 - shok_widgetpatch3)
        call widgetPatchOneLoadFn
        mov word [eax+shok_widgetpatchOff4], shok_widgetpatchOff4Cmp           ;test esi, esi
.done:
        retn