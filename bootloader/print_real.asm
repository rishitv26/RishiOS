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

debug:
    mov bx, debugmes
    call print
    jmp $

debug_cont:
    mov bx, debugmes
    call print

print_textm: ; use si instead of bx...
    pusha
    mov ax, 0xb800
    mov es, ax
    xor di, di

str_print:
    mov al, [si] ; si cant be compared directly
    mov [es:di], al
    mov byte[es:di+1], 0xa
    cmp al, 0
    je e
    add di, 2
    inc si
    jmp str_print

debugmes: db "this is a debug message...", 0