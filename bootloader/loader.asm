[bits 16]
[org 0x7e00] ; this is in sector 1

start:
    mov [driveid], dl 

    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb nosup ; check for some other random thing that is needed

    mov eax, 0x80000001
    cpuid ; retrieve cpu info in edx
    test edx, (1 << 29) ; check if 1gb page support is there, needed by kernel
    jz nosup
    test edx, (1 << 26) ; check if 1gb page support is there, needed by kernel
    jz nosup

    mov bx, mes
    call print ;; we can go to long mode!!

; loadkernel:
;     mov si, readpack ; this is structure for loading proccess
;     mov word[si], 0x10 ; size
;     mov word[si + 2], 5 ; number of sectors to be loaded
;     mov word[si + 4], 0x7e00 ; where our next disk is
;     mov word[si + 6], 0
;     mov dword[si + 8], 1 ; which sector exactly is going to be loaded
;     mov dword[si + 0xc], 0
;     jc loaderror ; if it fails, print error and halt.
;     mov dl, [driveid]
;     mov ah, 0x42
;     int 0x13
;     jc loaderror
;     mov dl, [driveid]
;     jmp 0x7e00 ; far jump to next section of our code

loaderror:
    mov bx, nosup
    call print

end:
    hlt
    jmp end

nosup:
    mov bx, nosupmes
    call print 
    jmp end

%include 'print_real.asm'
mes: db "[*] LONG MODE IS SUPPORTED...", 0
nosupmes: db "[*] ERROR: This device is not elligible for RishiOS.", 0
driveid: db 0