;
; The Loader: it executes neccessary disk tests, an and jumps to long mode...
;
;; tasks: check for unallocated memory, start our kernel session, check for a20 line,
;;        sets video mode to text, loads gdt and idt initial tables, jumps to protected mode
;;        enables paging, updates gdt for 64-bit mode, and finally jumps to long mode!
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
    mov dword[0x9000], 0
    mov edi, 0x9008
    xor ebx, ebx ; all of that is to prepare interrupt 15h
    int 0x15 ; This is there to get all free memory to us, to be used by kernel...
    jc loaderror ; there was an error in retrieving memory map

startmeminfo:
    add edi, 20 ; increment by 20 to check if next block is usable...
    inc dword[0x9000]
    test ebx, ebx
    jz finishmeminfo

    mov eax, 0xe820
    mov edx, 0x534d4150
    mov ecx, 20
    int 0x15; this is to check if memory block is availible to us...
    jc startmeminfo ; keep on going if there is more memory availible to us...
    
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

loadtables: ; we are going to load the GDT (specifies OS permissions) and IDT (Tells CPU what to do at each interrupt)
    cli ; disable normal interrupts
    lgdt [gdtptr] ; load our custom gdt
    lidt [idtptr] ; load our dummy idt (this will also make computer reset if there is a critical hardware error...)

    mov eax, cr0
    or eax, 1
    mov cr0, eax ;; YAY! we are in protected mode!!!

    jmp 8:pmentry ;; jump to protected mode entry:

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
mes: db "[*] LONG MODE IS SUPPORTED..."
    times (80 - ($ - mes)) db " " 
    db 0
nosupmes: db "[*] ERROR: This device is not elligible for RishiOS."
    times (80 - ($ - nosupmes)) db " " 
    db 0
kernel: db "[*] KERNEL HAS BEEN LOADED INTO MEMORY..."
    times (80 - ($ - kernel)) db " " 
    db 0
error: db "[*] THERE WAS AN ERROR DURING BOOT PROCCESS..."
    times (80 - ($ - error)) db " " 
    db 0
a20on: db "[*] a20 IS ON..."
    times (80 - ($ - a20on)) db " " 
    db 0
a20off: db "[*] ERROR: a20 IS OFF... MUST SET IT ON."
    times (80 - ($ - a20off)) db " " 
    db 0
driveid: db 0
readpack: times 16 db 0

;; Important Tables ----------------------------------------------->
gdt32: ; this is the gdt in its full glory...
    dq 0
code32: ; this tells the cpu how we want our code to be treated
    dw 0xffff
    dw 0
    db 0
    db 0x9a
    db 0xcf
    db 0
data32: ; this tells the cpu how we want our data to be treated
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 0xcf
    db 0
gdtptrlen: equ $ - gdt32

gdtptr: ; this is the pointer that helps the computer get to our custom gdt...
    dw gdtptrlen - 1
    dd gdt32

idtptr: ; this is a dummy pointer that is just there to tell the computer to screw its old IDT
    dw 0
    dd 0

; PROTECTED MODE STUFF ------------------------------------------------------------>
[bits 32] ;; From now on, everything is in 32 bit...
pmentry:
    mov ax, 0x10 ;; use this to initialize all segment registers
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x7c00

pm: ;; This code will run in 32 bit protected mode...
enablepaging:
    cld
    mov edi, 0x80000
    xor eax, eax
    mov ecx, 0x10000 / 4
    rep stosd

    mov dword[0x80000], 0x81007
    mov dword[0x81000], 10000111b ;; all of that enables paging, memory management in long mode...
updategdt:
    lgdt [gdt64ptr] ;; update our gdt for 64-bit mode
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax
    mov eax, 0x80000
    mov cr3, eax ;; neccesary random thing for 64-but mode
longmode: ; Finally! Long Mode!
    mov ecx, 0xc0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax
    ;; YAY! WE ARE IN LONG MODE, YIPI YAHOI!
    jmp 8:lmentry

pmend:
    hlt
    jmp pmend

;; functions and declarations:
Message: db "[*] Protected Mode Entry Successfull..."
    times (80 - ($ - Message)) db " " 
    db 0

gdt64: ;; our new and last GDT for long mode!
    dq 0
    dq 0x0020980000000000
gdt64len: equ $-gdt64

gdt64ptr:
    dw gdt64len
    dd gdt64

;; LONG MODE STUFF ---------------------------------------------------------------->
[bits 64]
lmentry:
    mov rsp, 0x7c00 ;; initialize stack pointer
lm:
    ;; jump to the kernel
    cld
    mov rdi, 0x200000
    mov rsi, 0x10000
    mov rcx, 51200 / 8
    rep movsq

    jmp 0x200000 ; Far jump to our beautifull kernel!
