section luaTable
        tableEntry addArchive,      "AddArchive"
        tableEntry removeArchive,   "RemoveArchive"
        
section strings
mapPathFmt  db "%s\%s"        ; includes .s5x 
dotS5x      db ".s5x", 0

section code
addArchive:
        pushad
        mov ebx, [esp + 24h]
        
        sub esp, 100h
        mov ebp, esp
        
        push 1
        push ebx
        call shok_lua_tostring
        test eax, eax
        jnz .loadArchive
        
        mov ecx, [84EF60h]          ;Framework_CMain Instance
        cmp dword  [ecx+2Ch], 10h   ; len < 16
        jb .embeddedStr
        mov ecx, [ecx+18h]
        jmp .combinePath
.embeddedStr:
        add ecx, 18h
        
        
.combinePath:
        push ecx
        push 779F88h                ;"maps\user\0"
        push mapPathFmt
        push ebp
        call sprintf
        add esp, 4*4
        
        push ebp                    ; relPath
        lea ebp, [ebp+80h]
        push ebp                    ; absPath
        mov dword ecx, [88F088h]
        mov edx, [ecx]
        call [edx+30h]              ; makePathAbsolute()
        mov eax, ebp
        
.loadArchive:
        push 1                      ; at top flag
        push eax                    ; filename
        
        mov dword ecx, [88F088h]
        mov edx, [ecx]
        call [edx+18h]              ; loadArchive
        
        add esp, 100h
        popad
        xor eax, eax
        retn
        
removeArchive:                          ; [[[[88F088h]+8]]+0ch]
        pushad
        mov ebx, [esp + 24h]
        
        mov dword esi, [88F088h]        ; FileSystemMgr
        mov eax, [esi+8]                ; begin ptr
        mov eax, [eax]                  ; top entry
        mov eax, [eax+0Ch]              ; string with filename
        
        push 763058h                    ; ".s5x\0"
        push eax
        call strstr
        add esp, 8
        
        test eax, eax
        jz .notfound                    ; remove only .s5x archives!
        
        mov ecx, esi
        mov edx, [ecx]
        call [edx+28h]                  ; remove top archive
        
.notfound:
        popad
        xor eax, eax
        retn