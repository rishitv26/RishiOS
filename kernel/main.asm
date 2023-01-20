[bits 64]
[org 0x200000] ;; this is our kernel entry point...

start:
    mov rdi, idttable

    mov rax, handler0 ;; this is our handler for interrupt 0, (divide by 0)
    mov [rdi], ax
    shr rax, 16
    mov [rdi + 6], ax 
    shr rax, 16
    mov [rdi + 8], eax ;; all of this is to init the idt

    mov rax, timer ;; this is our handler for the hardware timer interrupt...
    add rdi, 32 * 16
    mov [rdi], ax
    shr rax, 16
    mov [rdi + 6], ax 
    shr rax, 16
    mov [rdi + 8], eax ;; all of this is to init the idt

    lgdt [gdt64ptr] ;; load another gdt specific to our kernel...
    lidt [idtptr] ;; load yet another idt!

    push 8
    push kernelentry
    db 0x48
    retf ; far return...

kernelentry: ;; our lovely little kernel in its full glory!
    mov byte[0xb8000], 'K'
    mov byte[0xb8001], 0xa

    xor eax, eax
    div eax ;; to test for interrupt 0

initpit: ;; setup external interrupts...
    mov al, (1 << 2) | (3 << 4)
    out 0x32, al

    mov ax, 1193182 / 100 ; 100 is the Hz signal that we want... (changable)
    out 0x40, al
    mov al, ah
    out 0x40, al ;; clock is set
intpic:   ;; setup interrupt controller:
    mov al, 0x11
    out 0x20, al
    out 0xa0, al

    mov al, 32
    out 0x21, al
    mov al, 40
    out 0xa1, al

    mov al, 4
    out 0x21, al
    mov al, 2
    out 0xa1, al

    mov al, 1
    out 0x21, al
    out 0xa1, al
    ;; PIC is initialized, YIPI YAHOI!

    mov al, 11111110b
    out 0x21, al
    mov al, 11111111b
    out 0xa1, al
    ;; yay!, just need to enable interrupts:
    sti 

end:
    hlt
    jmp end

;; functions and definitions -------------------------------------------------------->
handler0: ;; divide by zero handler...
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15 ;; save state of all these registers... they are used to bring cpu back to normal from exception panic...

    mov byte[0xb8000], '0' ;; let us know exception has occured...
    mov byte[0xb8001], 0xc

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax ;; update our changed to register after cpu has been handled with...

    jmp end
    iretq

timer:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15 ;; save state of all these registers... they are used to bring cpu back to normal from exception panic...

    mov byte[0xb8010], 'T' ;; let us know exception has occured...
    mov byte[0xb8011], 0xe

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax ;; update our changed to register after cpu has been handled with...

    jmp end
    iretq

gdt64: ;; our new and last GDT for long mode!
    dq 0
    dq 0x0020980000000000
gdt64len: equ $-gdt64
gdt64ptr:
    dw gdt64len
    dq gdt64


idttable:
    %rep 256 ;; define all 256 interupts (make them exist)
        dw 0
        dw 0x8
        db 0
        db 0x8e
        dw 0
        dd 0
        dd 0
    %endrep
idtlen: equ $-idttable
idtptr:
    dw idtlen
    dq idttable
