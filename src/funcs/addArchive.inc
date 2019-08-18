section luaTable
        tableEntry addArchive,          "AddArchive"
        tableEntry removeArchive,       "RemoveArchive"
        tableEntry getTopArchivePath,   "GetTopArchive"
        
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
        
        mov ecx, [shok_Framework_CMainInstance]
        cmp dword  [ecx+2Ch], 10h   ; len < 16
        jb .embeddedStr
        mov ecx, [ecx+18h]
        jmp .combinePath
.embeddedStr:
        add ecx, 18h
        
        
.combinePath:
        push ecx
        push string_mapsUser                ;"maps\user\0"
        push mapPathFmt
        push ebp
        call sprintf
        add esp, 4*4
        
        push ebp                    ; relPath
        lea ebp, [ebp+80h]
        push ebp                    ; absPath
        mov dword ecx, [shok_FileSystemMgr]
        mov edx, [ecx]
        call [edx+shok_FileSystemMgrVtO_MakePathAbsloute]              ; makePathAbsolute()
        mov eax, ebp
        
.loadArchive:
        push 1                      ; at top flag
        push eax                    ; filename
        
        mov dword ecx, [shok_FileSystemMgr]
        mov edx, [ecx]
        call [edx+shok_FileSystemMgrVtO_LoadArchive]              ; loadArchive
        
        add esp, 100h
        popad
        xor eax, eax
        retn
        
removeArchive:                          ; [[[[88F088h]+8]]+0ch]
        pushad
        mov ebx, [esp + 24h]
        
        mov dword esi, [shok_FileSystemMgr]        ; FileSystemMgr
        mov eax, [esi+shok_FileSystemMgrOO_BeginPointer]                ; begin ptr
        mov eax, [eax]                  ; top entry
        mov eax, [eax+shok_FileSystemMgrEntryOO_Filename]              ; string with filename
        
        push string_dots5x                    ; ".s5x\0"
        push eax
        call strstr
        add esp, 8
        
        test eax, eax
        jz .notfound                    ; remove only .s5x archives!
        
        mov ecx, esi
        mov edx, [ecx]
        call [edx+shok_FileSystemMgrVtO_RemoveTopArchive]                  ; remove top archive
        
.notfound:
        popad
        xor eax, eax
        retn

getTopArchivePath:
        pushad
        
        mov dword esi, [shok_FileSystemMgr]
        mov eax, [esi+shok_FileSystemMgrOO_BeginPointer]
        mov eax, [eax]
        mov eax, [eax+shok_FileSystemMgrEntryOO_Filename]
        
        push eax
        push ebx
        call shok_lua_pushstring
        
        popad
        mov eax, 1
        retn


