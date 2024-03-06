; contains all user code for all system calls:

section .text
[global writeu]
[global sleepu]

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
