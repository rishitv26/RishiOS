; boot code!
[org 0x7c00]
[bits 32]
mov ah, 0x0e ; so we can print in real mode

;; disk load:
diskload:
    push dx
    mov ax, 0
    mov es, ax
    mov ah, 2
    mov al, 1
    mov ch, 0
    mov dh, 0
    mov cl, 0
    mov bx, 0x7e00
    int 0x13
    jc disk_error
    pop dx
    cmp dh, al
    jne disk_error

disk_error:
    mov bx, message
    call print
    jmp diskload

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

message: db "disk error, retrying disk load (interupt 0x13 failed)...", 0

;; helping functions:
debug:
    mov bx, debugmsg
    call print
    jmp $

debugmsg: db "this is a debug message", 0

times 510-($-$$) db 0
db 0x55, 0xaa