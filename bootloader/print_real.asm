print:
    pusha
    mov ah, 0x0e

string_print:
    mov al, [bx] ; bx cant be compared directly
    cmp al, 0
    je e
    int 0x10
    inc bx
    jmp string_print

e:
    popa
    ret