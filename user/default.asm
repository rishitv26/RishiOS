section .text
[extern main]
[extern exitu]
[global start]

start:
    call main
    call exitu
    jmp $
