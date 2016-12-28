section luaTable
        tableEntry getTerrainInfo,      "GetTerrainInfo"
;       tableEntry setBlocking,         "SetBlocking"
;       tableEntry setSector,           "SetSector"

section code

toTerrainCoords:          ; ebx = L, ebp = {float[2],int[2]}, ret = Zflag
        
        mov eax, [895DACh]  ; CGLEGameLogic global instance
        mov esi, [eax+4*9]  ; CGLELandscape
        
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
        call 585C21h        ; converts floatXY to terrain grid coordinates
        
        push dword [ebp+12]
        push dword [ebp+8]
        mov ecx, [esi+4]
        call 449D52h
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
        mov ecx, [esi+28]   ; CGLETerrainHiRes
        
        mov eax, [ebp+12]   ; y
        inc eax
        imul eax, [ecx+28]  ; (y+1)*terrWidth
        add eax, [ebp+8]    ; (y+1)*terrWidth + x
        
        mov edx, [ecx + 8]  ; terrain height table
        lea eax, [edx + 2*eax + 2]
        movzx eax, word [eax]
        
        push eax
        push ebx
        call shok_lua_pushint
        
        mov eax, [8581ECh]  ; CGlobalsLogicEx global instance
        mov eax, [eax+24]
        mov ecx, [eax+4]    ; blocking table
        
        mov eax, [ebp+12]   ; y
        imul eax, [898B74h] ; y*global map size
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
        
        mov eax, [895DACh] ;GameLogic_Instance_895DAC
        mov eax, [eax+24h]
        mov esi, [eax+20h] ;CGLETerrainLowRes
        
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
        imul edx, [esi+2Ch]     ;(y+1)*len
        pop eax
        add edx, eax            ;x+(y+1)*len
        imul edx, 4
        
        mov ecx, [esi+8]        ;base
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