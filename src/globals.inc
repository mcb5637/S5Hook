
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
shok_Framework_CMainOOLen       equ 2Ch
shok_Framework_CMainOOUnknown1  equ 18h     ; TODO HE also check some offsets might be left
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

shok_GetGUIMgr                  equ 525622h
shok_GUIMgrOONetEvent           equ 28h

shok_GetFontIdMgr               equ 5593ADh
shok_GetFontObject              equ 5597C7h ; (this, id) TODO HE call by vtable?
shok_FontOOSize                 equ 4
shok_FontOOZOffset              equ 8
shok_FontOOSpacing              equ 0Ch

shok_EntityHurtEntity           equ 49F358h

shok_postNetEvent               equ 525D77h

GGL_CNetEventExtractResourceVt  equ 77061Ch
EGL_CNetEvent2EntitiesVt        equ 76DD60h
EGL_CNetEventEntityAndPosVt     equ 76DD50h
EGL_CNetEventEntityIDVt         equ 766C28h
EGL_CNetEventEntityIDAndPlayerIDVt  equ 766C38h
EGL_CNetEventEntityIDAndIntegerVt   equ 766C48h
GGL_CNetEventCannonCreatorVt    equ 7705ECh

shok_globalEntityManager        equ 897558h
shok_globalEntityManagerOOCount equ 4
shok_globalEntityManagerOOStartEntityTable  equ 6*4

shok_EGL_CGLEEntityOO_Id        equ 8
shok_EGL_CGLEEntityOO_PlayerId  equ 18h
shok_EGL_CGLEEntityOO_TypeId    equ 10h

string_mapsUser                 equ 779F88h
string_dots5x                   equ 763058h

shok_CGLEEntityDistancePredicate_ctor_579F9A     equ 579F9Ah ; thiscall CGLEEntityDistancePredicate_ctor_579F9A(<ecx> this, float centerPosXY[], float r)
shok_CGLEEntityPredicateArea_ctor_579EF8         equ 579EF8h ; thiscall CGLEEntityPredicateArea_ctor_579EF8(<ecx> this, float posA[], float posB[])
shok_CPlayerPredicate           equ 77B900h    ;CPlayerPredicate
shok_CGLEEntityTypePredicate    equ 766CA0h    ;CGLEEntityTypePredicate
shok_CGLIsBuildingPredicate     equ 77046Ch    ;CGLIsBuildingPredicate
shok_CEntityCategoryPredicate   equ 774E84h    ;CEntityCategoryPredicate
shok_CUpgradeCategoryPredicate  equ 777964h    ;CUpgradeCategoryPredicate
shok_CResourceProviderPredicate equ 76FF44h    ;CResourceProviderPredicate
shok_CSectorPredicate           equ 772B24h    ;CSectorPredicate

shok_LoadGUIGetObject           equ 5563CCh
shok_LoadGUI                    equ 556464h
shok_setHighPrecision           equ 5C8451h

shok_teleportentity             equ 57BC78h     ; thiscall (<ecx> this, float posXY[])
shok_logicMoveSettler           equ 4D8B01h     ; cdecl luaapi

shok_IsSettler                  equ 4A2B62h     ; cdecl (ep) -> ep


shok_CProjectileEffectCreator   equ 774698h
shok_createprojectileVTP        equ 895DACh
shok_createprojectileVtO        equ 05Ch
shok_EffectManagerInstance      equ 0898144h
shok_EffectManagerEffectExists  equ 04FAABDh
shok_EffectManagerEffidToObj    equ 04FAAE3h    ; vto?

shok_EGL_CEffectOOEffectID      equ 17h*4
shok_EGL_CEffectOOEffectType    equ 14h*4
shok_EGL_CEffectOOEffectSX      equ 22h*4
shok_EGL_CEffectOOEffectSY      equ 23h*4
shok_EGL_CEffectOOEffectTX      equ 26h*4
shok_EGL_CEffectOOEffectTY      equ 27h*4
shok_EGL_CEffectOOEffectAttID   equ 2Fh*4
shok_GGL_CArrowEffectOOEffectTarID equ 30h*4
shok_GGL_CArrowEffectOOEffectDamage equ 31h*4
shok_GGL_CCannonBallEffectOOEffectDamage equ 32h*4
shok_GGL_CCannonBallEffectOOAoERange equ 33h*4

shok_GGL_CArrowEffectVt         equ 778E24h
shok_GGL_CCannonBallEffectVt    equ 777690h
shok_EGL_CFlyingEffectVtOHitCallback equ 24h
shok_GGL_CArrowEffect_HitCallback equ 511336h
shok_GGL_CCannonBallEffect_HitCallback equ 4FF476h

shok_ReloadEntitiesPtr          equ 84EF60h
shok_ReloadEntitiesO1           equ 258h
shok_ReloadEntitiesO2           equ 0Ch
shok_ReloadEntitiesFunc         equ 5B9A97h

shok_str_mapsexternalmap        equ 779FFCh
shok_reloadCutsceneObj          equ 0A0344Ch
shok_reloadCutsceneVtOReload    equ 0Ch

shok_CGLEGameLogicObj           equ 895DACh
shok_CGLELandscapeOO            equ 4*9
shok_floatXYToTerrainGrid       equ 585C21h
shok_toTerrainCoordMysteriousFunc equ 449D52h
shok_CGLELandscapeOOCGLETerrainHiRes equ 28
shok_CGLETerrainHiResOOWidth    equ 28
shok_CGLETerrainHiResOOHeightTab equ 8
shok_CGlobalsLogicExObj         equ 8581ECh
shok_CGlobalsLogicExOff1        equ 24
shok_CGlobalsLogicExOff2        equ 4
shok_GlobalMapSize              equ 898B74h
shok_getSector                  equ 577461h     ; (x, y) -> s
shok_CGLEGameLogicOOCGLETerrainLowRes equ 24h
shok_CGLEGameLogicOOCGLETerrainLowRes2 equ 20h
shok_CGLEGameLogicOOCGLETerrainLowResOOLen equ 2Ch
shok_CGLEGameLogicOOCGLETerrainLowResOOBase equ 8

shok_CGLGameLogicOP             equ 85A3A0h
shok_CGLGameLogicOO1            equ 40
shok_CGLGameLogic_getPlayerStatus equ 4A91BCh
shok_playerstatusOOCBuildingUpgradeManager equ 792
shok_CBuildingUpgradeManagerAdd equ 4B3D42h
shok_CBuildingUpgradeManagerOO equ 20h
shok_CBuildingUpgradeManagerGetObj equ 4CA5A2h
shok_CBuildingUpgradeManagerGetObjOOScholars equ 0
shok_CBuildingUpgradeManagerGetObjOO0 equ 4

shok_GetWidgetObjF1             equ 55AE12h
shok_GetWidgetObjF2             equ 558473h
shok_GetWidgetObjF3             equ 558966h
shok_widgetOOx                  equ 14h
shok_widgetOOy                  equ 18h
shok_widgetOOxSiz               equ 1Ch
shok_widgetOOySiz               equ 20h
shok_widgetpatch1               equ 5617A7h
shok_widgetpatch2               equ 5617B8h
shok_widgetpatch3               equ 5617C9h
shok_widgetpatch4               equ 5617DAh
shok_widgetpatch5               equ 59BD99h
shok_widgetpatch6               equ 59BD81h
shok_widgetpatchOff1            equ 1
shok_widgetpatchOff2            equ 5
shok_widgetpatchOff2Cmp         equ 04EBh
shok_widgetpatchOff3            equ 5
shok_widgetpatchOff3Cmp         equ 0EBh
shok_widgetpatchOff4            equ 11
shok_widgetpatchOff4Cmp         equ 0F685h


; TODO HE  changeString: no idea how this works
; TODO HE  customnames: some funky jmp hacking
; TODO HE  hurtentity: maybe do better func hacking
; TODO HE  motivation: wtf is yoq doing there?
; TODO HE  remove or fix mousetrigger
; TODO HE  no idea how musicfix works
; TODO HE  no idea how osi works

struc ProjDef
    .creatorType:   resd 1
    .effectType:    resd 1
                    resd 1
    .playerID       resd 1
    
    .startXcopy:    resd 1
    .startYcopy:    resd 1
    .startX:        resd 1  ;f
    .startY:        resd 1  ;f
    .targetX:       resd 1  ;f
    .targetY:       resd 1  ;f
    
                    resd 1
    .attackerID:    resd 1
    .targetID:      resd 1
    .damage:        resd 1  ;int
    .radius:        resd 1  ;f
    
                    resd 3
    .size:
endstruc

charCall    equ 40754Dh     ; jmp address of call
charOrig    equ 54E649h


keyCall     equ 40757Eh     ; jmp address of call
keyOrig     equ 54E82Fh

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