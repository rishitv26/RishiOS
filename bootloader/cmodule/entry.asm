; file used to enter into emain. Assembly wrapper file

section .text
[extern emain]
[global start]

start:
    mov rsp, 0xffff800000200000
    call emain

term: ;; end of program.
    hlt
    jmp term
