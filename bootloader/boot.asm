;
; This is where the operating system starts... the begining of everything...
;
;; tasks: read second sector into RAM, and jump to a less limited file, loader.asm

[bits 16]
[org 0x7c00] ; MBR boot location

start: ; start command.
    mov bx, mes
    call print
    xor ax, ax ; initialize our lovely stack
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

testext:
    mov [driveid], dl
    mov ah, 0x41
    mov bx, 0x55aa
    int 0x13
    jc nosupport ; do something if disk extension is not supported
    cmp bx, 0xaa55
    jne nosupport ; do something if disk extension is not supported

loadloader:
    mov si, readpack ; this is structure for loading proccess
    mov word[si], 0x10 ; size
    mov word[si + 2], 5 ; number of sectors to be loaded
    mov word[si + 4], 0x7e00 ; where our next disk is
    mov word[si + 6], 0
    mov dword[si + 8], 1 ; which sector exactly is going to be loaded
    mov dword[si + 0xc], 0
    jc loaderror ; if it fails, print error and halt.
    mov dl, [driveid]
    mov ah, 0x42
    int 0x13
    jc loaderror
    mov dl, [driveid]
    jmp 0x7e00 ; far jump to next section of our code

loaderror:
nosupport:
    mov bx, nosup
    call print
end:
    hlt ; this will be resumed with a interupt
    jmp end

%include 'print_real.asm'
mes: db "[*] booting...", 0
nosup: db "[*] error.", 0
driveid: db 0
readpack: times 16 db 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
times (0x1be-($-$$)) db 0

db 80h ; boot indicater for bios                     ;
db 0,2,0 ; starting part of CHS                      ;
db 0f0h ; type of CHS                                ;
db 0ffh, 0ffh, 0ffh ; end of CHS                     ;
dd 1 ; tells that this is the start AKA boot sector  ; This whole thing is to trick some BIOS's
dd (20 * 16 * 63 - 1) ; calculate the size via math  ; that this is a hard disk
                                                     ;
times (16*3) db 0 ; more zero's                      ;
                                                     ;
db 0x55
db 0xaa ; these seperate this boot code from rest of memory. 
; this is what computer looks for on startup.