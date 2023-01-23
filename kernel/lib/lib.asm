section .text
[global memset]
[global memcpy]
[global memmove]
[global memcmp]

memset: ; rdi (buffer) rsi (value) rdx (size)
    cld ; for memory nonsense 
    mov ecx, edx ;  -|
    mov al, sil  ;  -|- this is for initializing the fields using parameters passes in...
    rep stosb ; instead of using C, this is more effecient
    ret

memcmp:
    cld
    xor eax, eax ; eax is 0
    mov ecx, edx
    repe cmpsb ; compare mem
    setnz al ; set answer to al
    ret ; return

memcpy: ; both of the functions do practicaly the same thing
memmove:
    cld
    cmp rsi, rdi ; see what we need to do
    jae .copy
    mov r8, rsi ; just move mem
    add r8, rdx
    cmp r8, rdi
    jbe .copy

.overlap:
    std ; more memory nonsense
    add rdi, rdx
    add rsi, rdx
    sub rdi, 1
    sub rsi, 1

.copy: ; just copy mem
    mov ecx, edx
    rep movsb ; copy the src to dest
    cld
    ret
