bits 32
section payload
%include 'globals.inc'

org hookBase


entryPoint      dd installer
copySize        dd payloadSize


section strings align=1             ; const strings
sS5Hook         db "S5Hook", 0
sVERSION        db "Version", 0
sS5HookVersion  db "2.2", 0

section luaTable align=1
luaFuncTable:
                tableEntry unload,             "Unload"
                tableEntry triggerInt3,        "Break"
                ; dd 0 ; omit because of cnTableRef ;)

section globalVars align=1

section code align=1
installer:
        
        ; restore SP after ROP loader
        lea ecx, [esp - 184h]           ; eObj
        mov esp, [ecx + 12]             ; restore esp from xchg
        add esp, 3*4                    ; undo "push, push, call esi"
        pushad
        
        mov al, [hookRsrcFlag]
        test al, al
        jnz .rtStateExists
        call [lua_open]
        mov [rtState], eax
        mov byte [hookRsrcFlag], 1
.rtStateExists:
    
        mov ebx, [luaHandle]
        
        push dword sS5Hook
        push dword luaFuncTable
        call registerFuncTable
        
        call startupSetup
        
        ; set S5Hook.Version
        push sS5Hook
        push ebx
        call shok_lua_pushstring
        
        push LUA_GLOBALSINDEX
        push ebx
        call [lua_rawget]
        
        push sVERSION
        push ebx
        call shok_lua_pushstring
        
        push sS5HookVersion
        push ebx
        call shok_lua_pushstring
        
        push -3
        push ebx
        call [lua_settable]
        
        push -2
        push ebx
        call [lua_settop]      
        
        add esp, 4*6
        
        
        
        ; patch functions to unload s5hook
        mov eax, leaveJump                  ; create jump at this location 
        mov byte [eax], 0E9h                ; opcode jmp
        mov dword [eax+1], leaveOffset      ; relative jmp target
        
        mov eax, loadJump                   ; create jump at this location 
        mov byte [eax], 0E9h                ; opcode jmp
        mov dword [eax+1], loadOffset       ; relative jmp target
        
        
        popad
        retn 4


%include 'funcs/globalFuncs.inc'
%include 'funcs/musicfix.inc'
%include 'funcs/osi.inc'
%include 'funcs/runtimeStore.inc'
%include 'funcs/changeString.inc'
%include 'funcs/log.inc'
%include 'funcs/addArchive.inc'
%include 'funcs/reloadCuts.inc'
%include 'funcs/loadGUI.inc'
%include 'funcs/evalLua.inc'
%include 'funcs/customNames.inc'
%include 'funcs/charTrigger.inc'
%include 'funcs/keyTrigger.inc'
%include 'funcs/mouseTrigger.inc'
%include 'funcs/motivation.inc'
%include 'funcs/reloadConfig.inc'
%include 'funcs/widget.inc'
%include 'funcs/projectile.inc'
%include 'funcs/terrain.inc'
%include 'funcs/imports.inc'
%include 'funcs/memory.inc'
%include 'funcs/iterator.inc'
%include 'funcs/upgrade.inc'
%include 'funcs/event.inc'
%include 'funcs/fonts.inc'
%include 'funcs/hurtentity.inc'
%include 'funcs/bits.inc'
    
leaveJump     equ    40AA1Fh
leaveOffset equ leaveGameHook - (leaveJump + 5)
leaveGameHook:                  ; jmp from 40AA1F
        lea edi, [esi+2A4h]     ; overwritten instruction
        pushad
        
        mov ebx, [luaHandle]
        call unpatchEverything
        
        popad
        jmp 40AA25h


loadJump     equ    40AA76h
loadOffset     equ loadGameHook - (loadJump + 5)
loadGameHook:                   ; jmp from 40AA76
        mov eax, 72A2C0h        ; overwritten instruction
        pushad
        
        mov ebx, [luaHandle]
        call unpatchEverything
        
        popad
        jmp 40AA7Bh
        
triggerInt3:
        int1
        xor eax, eax
        retn
        
unload:
    pushad
    call unpatchEverything
    
    push sS5Hook
    push ebx
    call shok_lua_pushstring
    
    push ebx
    call [lua_pushnil]
    
    push LUA_GLOBALSINDEX
    push ebx
    call [lua_settable]
    
    add esp, 3*4
    
    popad
    xor eax, eax
    retn

unpatchEverything:
section cleanup align=1
        retn
        
startupSetup:
section autorun align=1
        retn

payloadSize equ $ - hookBase