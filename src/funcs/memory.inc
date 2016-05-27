section luaTable
        tableEntry directAccessEntity,  "GetEntityMem"
        tableEntry directAccessPointer, "GetRawMem"

section autorun
        call daSetupFuncs
        
section globalVars
daMetaTableRef      dd 0

section code

daFuncs:
        tableEntry daGetInt,            "GetInt"
        tableEntry daGetFloat,          "GetFloat"
        tableEntry daSetInt,            "SetInt"
        tableEntry daSetFloat,          "SetFloat"
        tableEntry daGetString,         "GetString"
        tableEntry daCreateRelative,    "cr"
        dd 0
        
sMemTbl     db "__mem", 0
sDA_UD_ERR  db "use :", 0

daSetupFuncs:
        push sMemTbl
        push daFuncs
        call registerFuncTable
        
        push sMemTbl
        push ebx
        call shok_lua_pushstring
        
        push LUA_GLOBALSINDEX
        push ebx
        call [lua_rawget]
        
        push LUA_REGISTRYINDEX
        push ebx
        call [luaL_ref]
        mov [daMetaTableRef], eax
        
        add esp, 4*4
        
        retn


directAccessEntity:
        pushad
        mov ebx, [esp+24h]
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax
        call shok_eid2obj
        test eax, eax
        jnz .entityOK
        
        popad
        xor eax, eax
        retn
        
.entityOK:
        
        push eax
        call daCreateUDHead
        
        popad
        mov eax, 1
        retn


directAccessPointer:
        pushad
        mov ebx, [esp+24h]
        
        push 1
        push ebx
        call shok_lua_toint
        
        push eax
        call daCreateUDHead
        
        popad
        mov eax, 1
        retn

; two way entry func: daCreateUD, daCreateUDHead
daCreateUDHead:
        mov esi, [esp+4]
        
        push 8
        push ebx
        call [lua_newuserdata]
        lea edx, [eax+4]
        mov dword [eax], edx
        mov dword [edx], esi
        jmp daSetMT

daCreateUD:
        mov esi, [esp+4]
        
        push 4
        push ebx
        call [lua_newuserdata]
        mov dword [eax], esi
daSetMT:        
        push dword [daMetaTableRef]
        push LUA_REGISTRYINDEX
        push ebx
        call [lua_rawgeti]
        
        push -2
        push ebx
        call [lua_setmetatable]
        
        add esp, 7*4
        retn 4

daGetUdArg:
        push 1
        push ebx
        call shok_lua_touserdata
        test eax, eax
        jz .abort
        retn
.abort:
        push sDA_UD_ERR
        push ebx
        call [luaL_error] ; doesn't return
       ; retn

daCreateRelative: ;(ud, n)
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov esi, [eax]
        mov esi, [esi]      ; deref ptr
        
        push 2
        push ebx
        call shok_lua_toint
        
        lea eax, [esi + 4*eax]
        push eax
        call daCreateUD
        
        popad
        mov eax, 1
        retn

daSetFloat:
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov esi, [eax]
        
        push 2
        push ebx
        call shok_lua_tonumber
        
        fstp dword [esi]
        
        popad
        mov eax, 0
        retn
        
        
daSetInt:
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov esi, [eax]
        
        push 2
        push ebx
        call shok_lua_toint
        mov [esi], eax
        
        popad
        mov eax, 0
        retn


daGetFloat:
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov eax, [eax]
        
        fld dword [eax]
        call luaPushFloat
        
        popad
        mov eax, 1
        retn
        
        
daGetInt:
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov eax, [eax]
        
        push dword [eax]
        push ebx
        call shok_lua_pushint
        
        popad
        mov eax, 1
        retn


daGetString:
        pushad
        mov ebx, [esp+24h]
        
        call daGetUdArg
        mov eax, [eax]
        
        push dword [eax]
        push ebx
        call shok_lua_pushstring
        
        popad
        mov eax, 1
        retn