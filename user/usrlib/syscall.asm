; contains all user code for all system calls:

section .text
[global writeu]
[global sleepu]
[global exitu]
[global cleanup]
[global clearu]
[global keyboard_readu]
[global cmd_get_total_memoryu]

writeu:
    sub rsp,16 ; setup arguments for interrupt to be called.
    xor eax,eax

    mov [rsp],rdi
    mov [rsp+8],rsi

    mov rdi,2
    mov rsi,rsp
    int 0x80

    add rsp, 16 ; restore stack
    ret

sleepu:
    sub rsp, 8 ; setup arguments for interupt to be called
    mov eax, 1
    mov [rsp], rdi
    mov rdi, 1
    mov rsi, rsp

    int 0x80

    add rsp, 8 ; restore stack
    ret

exitu:
    mov eax, 2 ; syscall index number
    mov rdi, 0 ; no arguments passed

    int 0x80
    ret

cleanup:
    mov eax, 3
    mov rdi, 0

    int 0x80
    ret

clearu:
    mov eax, 4
    mov rdi, 0

    int 0x80
    ret

keyboard_readu:
    mov eax, 5
    xor edi, edi

    int 0x80
    ret

cmd_get_total_memoryu:
    mov eax, 6
    mov rdi, 0

    int 0x80
    ret

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

