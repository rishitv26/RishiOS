[bits 64]
[org 0x200000] ;; this is our kernel entry point...

start:
    mov byte[0xb8000], 'K'
    mov byte[0xb8001], 0xf

end:
    hlt
    jmp end