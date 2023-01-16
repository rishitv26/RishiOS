; boot code!
[org 0x7c00]
mov ah, 0x0e ; so we can print in real mode

;; prepare for disk load:
mov bx, 0x1000
mov es, bx
mov bx, 0x0

mov dh, 0x0
mov dl, 0x0
mov ch, 0x0
mov cl, 0x02

read_disk:
    mov ah, 0x02 ; why is it freezing here?
    call debug
    mov al, 0x01
    int 0x13

    jc redo  ;; retry at disk read error

    mov ax, 0x1000 ;; set all segments to kernel, or whatever ax is supposed to be.
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x1000:0 ;; jump to kernel in next sector!

redo:
    mov bx, message
    call print
    jmp read_disk

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

debug:
    mov bx, debugmsg
    call print
    jmp $

message: db "RETRYING DISK LOAD", 0
debugmsg: db "this is a debug message", 0

times 510-($-$$) db 0
db 0x55, 0xaa