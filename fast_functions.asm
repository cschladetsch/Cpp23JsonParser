section .text
global fast_strcmp

fast_strcmp:
    mov     al, [rdi]
    cmp     al, [rsi]
    jne     .not_equal
    test    al, al
    jz      .equal
    inc     rdi
    inc     rsi
    jmp     fast_strcmp

.not_equal:
    movzx   eax, byte [rdi]
    movzx   ecx, byte [rsi]
    sub     eax, ecx
    ret

.equal:
    xor     eax, eax
    ret
