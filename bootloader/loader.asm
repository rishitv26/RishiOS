;
; The Loader: it executes neccessary disk tests, before jumping to long mode...
;
;; tasks: check for unallocated memory, start our kernel session, check for a20 line
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

loadkernel: ;; load our kernel:
    mov si, readpack 
    mov word[si], 0x10 
    mov word[si + 2], 100 
    mov word[si + 4], 0
    mov word[si + 6], 0x1000
    mov dword[si + 8], 6
    mov dword[si + 0xc], 0
    jc loaderror ; if it fails, print error and halt.
    mov dl, [driveid]
    mov ah, 0x42
    int 0x13
    jc loaderror
    mov dl, [driveid]

meminfo: ; See what sections of memory are usable for us.
    mov eax, 0xe820
    mov edx, 0x534d4150
    mov ecx, 20
    mov edi, 0x9000
    xor ebx, ebx ; all of that is to prepare interrupt 15h
    int 0x15 ; This is there to get all free memory to us, to be used by kernel...
    jc loaderror ; there was an error is retrieving memory map

startmeminfo:
    add edi, 20 ; increment by 20 to check if next block is usable...
    mov eax, 0xe820
    mov edx, 0x534d4150
    mov ecx, 20
    int 0x15; this is to check if memory block is availible to us...
    jc finishmeminfo
    
    test ebx, ebx
    jnz startmeminfo ; if we have more memory, keep on going...
    
finishmeminfo:
    call kernelsuccess

testa20: ; if a20 line is off, 0x107c00 = 0x7c00 (use this to test)
    mov ax, 0xffff
    mov es, ax
    mov word[ds:0x7c00], 0xa200
    cmp word[es:0x7c10], 0xa200
    jne a20complete ; This is to see whether a20 line is set...
    mov word[0x7c00], 0xb200
    cmp word[es:0x7c10], 0xb200
    je a20error
    ;; Yay! yipi Yahoi!

setvidmode:
    mov ax, 3
    int 0x10 ; set video mode to text mode...

    mov si, text
    call print_textm

end:
    hlt
    jmp end

;; function & variable definitions --------------------------------->
a20complete:
    xor ax, ax
    mov es, ax
    mov bx, a20on
    call print
    jmp setvidmode

a20error:
    mov bx, a20off
    call print
    jmp end

loaderror:
    mov bx, error
    call print 
    jmp end

nosup:
    mov bx, nosupmes
    call print 
    jmp end

kernelsuccess:
    mov bx, kernel
    call print
    ret

%include 'print_real.asm'
mes: db "[*] LONG MODE IS SUPPORTED...", 0
nosupmes: db "[*] ERROR: This device is not elligible for RishiOS.", 0
kernel: db "[*] KERNEL HAS BEEN LOADED INTO MEMORY...", 0
error: db "[*] THERE WAS AN ERROR DURING BOOT PROCCESS...", 0
a20on: db "[*] a20 IS ON...", 0
a20off: db "[*] ERROR: a20 IS OFF... MUST SET IT ON.", 0
text: db "[*] Yay! preparing for long mode...", 0
driveid: db 0
readpack: times 16 db 0