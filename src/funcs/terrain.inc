section luaTable
        tableEntry getTerrainInfo,      "GetTerrainInfo"
;       tableEntry setBlocking,         "SetBlocking"
;       tableEntry setSector,           "SetSector"

section code

toTerrainCoords:          ; ebx = L, ebp = {float[2],int[2]}, ret = Zflag
        
        mov eax, [shok_CGLEGameLogicObj]  ; CGLEGameLogic global instance
        mov esi, [eax+shok_CGLELandscapeOO]  ; CGLELandscape
        
        push 1
        push ebx
        call shok_lua_tonumber
        fstp dword [ebp]
        
        push 2
        push ebx
        call shok_lua_tonumber
        fstp dword [ebp+4]
        
        lea ecx, [ebp + 8]
        push ebp
        call shok_floatXYToTerrainGrid        ; converts floatXY to terrain grid coordinates
        
        push dword [ebp+12]
        push dword [ebp+8]
        mov ecx, [esi+4]
        call shok_toTerrainCoordMysteriousFunc
        test eax, eax       ; valid coords?
        
        retn

; height, blocking, sector = S5Hook.GetTerrainInfo(x, y)
getTerrainInfo:
        pushad
        mov ebx, [esp+24h]
        
                                ; float[2] posXY 
        sub esp, 4*4            ; int[2] posIntXY
        mov ebp, esp 
                
        call toTerrainCoords
        jz .abort
        
        ;esi is set in toTerrainCoords ;)
        mov ecx, [esi+shok_CGLELandscapeOOCGLETerrainHiRes]   ; CGLETerrainHiRes
        
        mov eax, [ebp+12]   ; y
        inc eax
        imul eax, [ecx+shok_CGLETerrainHiResOOWidth]  ; (y+1)*terrWidth
        add eax, [ebp+8]    ; (y+1)*terrWidth + x
        
        mov edx, [ecx + shok_CGLETerrainHiResOOHeightTab]  ; terrain height table
        lea eax, [edx + 2*eax + 2]
        movzx eax, word [eax]
        
        push eax
        push ebx
        call shok_lua_pushint
        
        mov eax, [shok_CGlobalsLogicExObj]  ; CGlobalsLogicEx global instance
        mov eax, [eax+shok_CGlobalsLogicExOff1]
        mov ecx, [eax+shok_CGlobalsLogicExOff2]    ; blocking table
        
        mov eax, [ebp+12]   ; y
        imul eax, [shok_GlobalMapSize] ; y*global map size
        add eax, [ebp+8]    ; y*ms + x
        movzx eax, byte [ecx + eax]
        
        push eax
        push ebx
        call shok_lua_pushint
        
        push dword [ebp+12] ; y
        push dword [ebp+8]  ; x
        call 577461h        ; GetSector(x, y)
        
        push eax
        push ebx
        call shok_lua_pushint
        
        
        ; terraintype
        push esi
        
        mov eax, [shok_CGLEGameLogicObj] ;GameLogic_Instance_895DAC
        mov eax, [eax+shok_CGLEGameLogicOOCGLETerrainLowRes]
        mov esi, [eax+shok_CGLEGameLogicOOCGLETerrainLowRes2] ;CGLETerrainLowRes
        
        mov ecx, 4
        
        mov eax, [ebp+8]        ;x (hires)
        cdq                     ;0-extend to edx
        idiv ecx                ;x/4 (lores)
        push eax
        
        mov eax, [ebp+12]       ;y (hires)
        cdq                     ;0-extend to edx
        idiv ecx                ;y/4 (lores)
        mov edx, eax
        
        inc edx                 ;(y+1)
        imul edx, [esi+shok_CGLEGameLogicOOCGLETerrainLowResOOLen]     ;(y+1)*len
        pop eax
        add edx, eax            ;x+(y+1)*len
        imul edx, 4
        
        mov ecx, [esi+shok_CGLEGameLogicOOCGLETerrainLowResOOBase]        ;base
        add ecx, 4 
        add ecx, edx 
        
        movzx eax, byte [ecx]
        push eax
        push ebx
        call shok_lua_pushint
        
        pop esi
        ; terraintype end
        
        
        add esp, 4*4 
        
        popad
        mov eax, 4
        retn
        
.abort:
        add esp, 4*4
        popad
        xor eax, eax
        retn

%if 0
setBlocking:
        pushad
        mov ebx, [esp+24h]
        
                                ; float[2] posXY 
        sub esp, 4*4            ; int[2] posIntXY
        mov ebp, esp 
                
        call toTerrainCoords
        jz .abort
        
        push 3
        push ebx
        call shok_lua_toint
        
        mov edx, [8581ECh]  ; CGlobalsLogicEx global instance
        mov edx, [edx+24]
        mov ecx, [edx+4]    ; blocking table
        
        mov edx, [ebp+12]   ; y
        imul edx, [898B74h] ; y*global map size
        add edx, [ebp+8]    ; y*ms + x
        mov byte [ecx + edx], al
        
.abort:
        add esp, 4*4
        popad
        xor eax, eax
        retn
        

setSector:
        pushad
        mov ebx, [esp+24h]
        
                                ; float[2] posXY 
        sub esp, 4*4            ; int[2] posIntXY
        mov ebp, esp 
                
        call toTerrainCoords
        jz .abort
        
        push 3
        push ebx
        call shok_lua_toint
                                            ; access as in GetSector (577461h)
        mov edx, [898B74h]                  ; global_mapsize
        imul edx, [ebp+12]                  ; y
        add edx, [ebp+8]                    ; x
        mov ecx, [9D756Ch]
        movzx edx, word [ecx+edx*2]
        lea edx, [edx+edx*4]                ;edx*5
        mov dword [93755Ch + edx*4], eax
        
.abort:
        add esp, 4*4
        popad
        xor eax, eax
        retn

%endif