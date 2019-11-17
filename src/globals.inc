
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

shok_getAnimIdFromName          equ 54F19Eh ; ecx=[shok_getAnimIdFromNameObjectP] pString -> int
shok_getAnimIdFromNameObjectP   equ 0A0C838h

shok_Framework_CMainInstance    equ 84EF60h
shok_FileSystemMgr              equ 88F088h
shok_FileSystemMgrVtO_MakePathAbsloute equ 30h ; (PrelPath, PabsPath)
shok_FileSystemMgrVtO_LoadArchive   equ 18h ; (atTopFlag, path)
shok_FileSystemMgrOO_BeginPointer   equ 8
shok_FileSystemMgrEntryOO_Filename  equ 0Ch
shok_FileSystemMgrVtO_RemoveTopArchive  equ 28h ; ()

shok_GGL_CWorkerBehaviorVt      equ 772B30h
shok_GGL_CWorkerBehavior_GetMaxWorktimeAdr equ 4CE866h

shok_EGL_CGLEEntityOO_BehaviorListStart equ 31*4
shok_EGL_CGLEEntityOO_BehaviorListEnd equ 32*4

string_mapsUser                 equ 779F88h
string_dots5x                   equ 763058h

; "global" vars
luaHandle       equ 853A9Ch ;ptr

                    ;Imports (ptr to funcs)
GetProcAddress  equ 7611D8h
GetModuleHandleA equ 7610D8h

lua_tonumber    equ 761220h
lua_dump        equ 00761224h
lua_rawequal    equ 00761228h
lua_isnumber    equ 0076122Ch
lua_pushlstring equ 00761230h
luaL_ref        equ 761234h
lua_rawgeti     equ 761238h
luaL_unref      equ 76123Ch
lua_tocfunction equ 00761240h
lua_dostring    equ 761244h
lua_load        equ 00761248h
lua_isstring    equ 0076124Ch
lua_bb_getuserstate equ 00761250h
lua_settable    equ 761254h
lua_pushcclosure equ 761258h
lua_pushstring  equ 76125Ch
lua_insert      equ 00761260h
lua_rawseti     equ 761264h
lua_newtable    equ 761268h
lua_close       equ 0076126Ch
lua_pushnil     equ 761270h
lua_next        equ 00761274h
lua_typename    equ 00761278h
lua_pushvalue   equ 76127Ch
lua_rawget      equ 761280h
lua_open        equ 761284h
lua_checkstack  equ 00761288h
luaopen_base    equ 0076128Ch
luaopen_string  equ 00761290h
luaopen_math    equ 00761294h
luaopen_table   equ 00761298h
lua_getgccount  equ 0076129Ch
lua_getgcthreshold equ 007612A0h
luaL_loadbuffer equ 7612A4h
lua_pushboolean equ 7612A8h
lua_toboolean   equ 7612ACh
lua_settop      equ 7612B0h
lua_type        equ 7612B4h
lua_gettable    equ 7612B8h
lua_gettop      equ 7612BCh
lua_pushlightuserdata equ 7612C0h
lua_pushnumber  equ 7612C4h
lua_pcall       equ 7612C8h
lua_tostring    equ 7612CCh     ; index, luaState
lua_touserdata  equ 7612D0h

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

LUA_TNIL            equ 0
LUA_TNUMBER         equ 3
LUA_TBOOLEAN        equ 1
LUA_TSTRING         equ 4
LUA_TTABLE          equ 5
LUA_TFUNCTION       equ 6

%define lua_upvalueindex(n) (LUA_GLOBALSINDEX-(n)) 

%macro  tableEntry  2           ; %1 = funcPtr, %2 = string
        dd %1
%%strLeng equ (%%afterStr - %%beforeStr)
        db %%strLeng
%%beforeStr:
        db %2, 0
%%afterStr:

%endmacro