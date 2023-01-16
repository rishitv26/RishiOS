print:
    pusha
    jmp print_func

print_func:
    mov al, [bx]
    int 0x10
    inc bx
    cmp al, 0
    jne print
    jmp end_print

end_print:
    popa
    ret
