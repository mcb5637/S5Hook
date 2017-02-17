
fakeJmpTable    equ 0A1DA38h
hookAllocdFlag  equ 0A1DA3Ch
rtState         equ 0A1DFF0h
hookRsrcFlag    equ 0A1DFF4h
loaderBase      equ 0856A28h
hookBase        equ 0A20000h

retToAfterSV    equ 592736h
                    ; Funcs     ; Args, push order ->
regLuaFunc      equ 59B22Bh     ; 0, funcPtr, strFuncName, strTable, luaState
writeToLog      equ 548268h     ; arg, format
addBBArchive    equ 545EA1h     ; 1, path
getStringValue  equ 556D2Eh     ; strKey
strncpy         equ 5C3AD0h     ; num, src, dest
memcpy          equ 5C1E30h     ; num bytes, source, dest
strlen          equ 5C1DA0h     ; str, after call len in eax
strdup          equ 5D8983h
strstr          equ 5C30E0h     ; cdecl char *strstr(char *str1, char *str2)
sprintf         equ 5C2D11h     ; cdecl int sprintf(char *buf, char *fmt, ...)
malloc          equ 5C4181h     ; cdecl void *malloc(size)
realloc         equ 5C4193h     ; cdecl void *realloc(ptr, newSize)
calloc          equ 5C4340h     ; cdecl void *calloc(size)
free            equ 5C2E2Dh     ; cdecl void free(ptr)


; "global" vars
luaHandle       equ 853A9Ch ;ptr

                    ;Imports (ptr to funcs)
GetProcAddress  equ 7611D8h
GetModuleHandleA equ 7610D8h

lua_open        equ 761284h
lua_gettop      equ 7612BCh
lua_settop      equ 7612B0h
lua_type        equ 7612B4h
lua_dostring    equ 761244h
lua_pushvalue   equ 76127Ch
lua_pushnil     equ 761270h
lua_pushstring  equ 76125Ch
lua_pushnumber  equ 7612C4h
lua_pushboolean equ 7612A8h
lua_pushlightuserdata equ 7612C0h
lua_pushcclosure equ 761258h
lua_newtable    equ 761268h
lua_gettable    equ 7612B8h
lua_settable    equ 761254h
lua_rawget      equ 761280h
lua_rawgeti     equ 761238h
lua_rawseti     equ 761264h
lua_tostring    equ 7612CCh     ; index, luaState
lua_toboolean   equ 7612ACh
lua_tonumber    equ 761220h
lua_touserdata  equ 7612D0h
lua_pcall       equ 7612C8h
luaL_unref      equ 76123Ch
luaL_ref        equ 761234h
luaL_loadbuffer equ 7612A4h

shok_lua_toint      equ 59BD81h
shok_lua_tonumber   equ 59BD99h
shok_lua_tostring   equ 59BDACh
shok_lua_tobool     equ 59BDD2h
shok_lua_touserdata equ 59BDBFh
shok_lua_pushint    equ 59BDEBh
shok_lua_pushbool   equ 59BE30h
shok_lua_pushstring equ 59BE1Dh

shok_eid2obj    equ 5825B4h

virtualAlloc    equ 761080h
virtualQuery    equ 761128h
virtualProtect  equ 761058h     ; lpdwOldProtection, dwNewProtection, dwStart, dwLen

 ; manual imports (imports.inc)
lua_setmetatable equ 0A1DB00h
lua_newuserdata  equ 0A1DB04h
luaL_error       equ 0A1DB08h

    ; constants
LUA_REGISTRYINDEX   equ (-10000)
LUA_GLOBALSINDEX    equ (-10001)

%define lua_upvalueindex(n) (LUA_GLOBALSINDEX-(n)) 

%macro  tableEntry  2           ; %1 = funcPtr, %2 = string
        dd %1
%%strLeng equ (%%afterStr - %%beforeStr)
        db %%strLeng
%%beforeStr:
        db %2, 0
%%afterStr:

%endmacro