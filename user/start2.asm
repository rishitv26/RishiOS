section .text
[extern main]
[global start]

start:
    call main
    jmp $