; [bits 64]
; [org 0x200000] ;; this is our kernel entry point...
section .data
;; functions and definitions -------------------------------------------------------->
; handler0: ;; all interupts will be implemented in C
;     push rax
;     push rbx
;     push rcx
;     push rdx
;     push rsi
;     push rdi
;     push rbp
;     push r8
;     push r9
;     push r10
;     push r11
;     push r12
;     push r13
;     push r14
;     push r15 ;; save state of all these registers... they are used to bring cpu back to normal from exception panic...

;     mov byte[0xb8000], '0' ;; let us know exception has occured...
;     mov byte[0xb8001], 0xc

;     pop r15
;     pop r14
;     pop r13
;     pop r12
;     pop r11
;     pop r10
;     pop r9
;     pop r8
;     pop rbp
;     pop rdi
;     pop rsi
;     pop rdx
;     pop rcx
;     pop rbx
;     pop rax ;; update our changed to register after cpu has been handled with...

;     jmp end
;     iretq

; timer:
;     push rax
;     push rbx
;     push rcx
;     push rdx
;     push rsi
;     push rdi
;     push rbp
;     push r8
;     push r9
;     push r10
;     push r11
;     push r12
;     push r13
;     push r14
;     push r15 ;; save state of all these registers... they are used to bring cpu back to normal from exception panic...

;     inc byte[0xb8020] ;; let us know exception has occured... (should change every clock cycle)
;     mov byte[0xb8021], 0xe
;     mov al, 0x20
;     out 0x20, al ;; done with interupt tasks...

;     pop r15
;     pop r14
;     pop r13
;     pop r12
;     pop r11
;     pop r10
;     pop r9
;     pop r8
;     pop rbp
;     pop rdi
;     pop rsi
;     pop rdx
;     pop rcx
;     pop rbx
;     pop rax ;; update our changed to register after cpu has been handled with...

;     iretq

; SIRQ: ;; suprious interrupts occur from external electronic interferences like EM waves...
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

    mov al, 11
    out 0x20, al
    in al, 0x20 ;; this is to check isr register to see if interrupt is sup or actual interrupt...
    test al, (1 << 7)
    jz .end ;; local label end
    mov al, 0x20
    out 0x20, al

    .end:
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

gdt64: ;; our new and last GDT for long mode!
    dq 0
    dq 0x0020980000000000
    dq 0x0020f80000000000
    dq 0x0000f20000000000
tssdes:
    dw tsslen - 1 ;; this nonsense is for the tss initialization in user mode...
    dw 0
    db 0
    db 0x89
    db 0
    db 0
    dq 0

gdt64len: equ $-gdt64
gdt64ptr:
    dw gdt64len
    dq gdt64

; sethandler: ;; will be implemented in C
;     mov [rdi], ax
;     shr rax, 16
;     mov [rdi + 6], ax 
;     shr rax, 16
;     mov [rdi + 8], eax ;; all of this is to init the interrupt
;     ret

; idttable: ;; will be reused in other files...
;     %rep 256 ;; define all 256 interupts (make them exist)
;         dw 0
;         dw 0x8
;         db 0
;         db 0x8e
;         dw 0
;         dd 0
;         dd 0
;     %endrep
; idtlen: equ $-idttable
; idtptr:
;     dw idtlen
;     dq idttable

; uend: ;; seperate end label for user since ring 3 must be called again and again...
;     jmp userentry ;; implemented in C

tss: ;; this will be our new interrupt handler in ring 3...
    dd 0
    dq 0x150000
    times 88 db 0
    dd tsslen
tsslen: equ $-tss

section .text
[extern main]
[global start]

start:
    ; mov rdi, idttable ;; will be implemented in C

    ; mov rax, handler0 ;; this is our handler for interrupt 0, (divide by 0)
    ; call sethandler

    ; mov rax, timer ;; this is our handler for the hardware timer interrupt...
    ; mov rdi, idttable + 32 * 16
    ; call sethandler
    
    ; mov rdi, idttable + 32*16 + 7*16 ;; these are for spurious (not real) interrupts
    ; mov rax, SIRQ
    ; call sethandler

    lgdt [gdt64ptr] ;; load another gdt specific to our kernel...
    ;; lidt [idtptr] ;; will be implemented in C

    mov rax, tss ;; load and init the tss to later be used in usermode!
    mov [tssdes + 2], ax
    shr rax, 16
    mov [tssdes + 4], al
    shr rax, 8
    mov [tssdes + 7], al
    shr rax, 8
    mov [tssdes + 8], eax
    mov ax, 0x20
    ltr ax ;; done with tss!

    ; xor eax, eax
    ; div eax ;; to test for interrupt 0

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
    ;sti ;; this will use tss btw...

    ; ;; jump from ring0 (highest previlige) to ring3 (lowest privelege)
    ; push 0x18 | 3
    ; push 0x7c00
    ; push 0x202
    ; push 0x10 | 3
    ; push userentry
    ; iretq ;; will be implemented in C

    push 8
    push kernelentry
    db 0x48
    retf ; far return...

kernelentry: ;; our lovely little kernel in its full glory!
    ; xor ax, ax
    ; mov ss, ax
    mov rsp, 0x200000 ; jump to kernel location...
    call main
    ; sti

end:
    hlt
    jmp end

; userentry: ;; will be implemented in C
;     mov ax, cs ;; test we are in user mode...
;     and al, 11b
;     cmp al, 3
;     jne uend ;; halt if not

;     ;; we are is user mode!!!
;     inc byte[0xb8012]
;     mov byte[0xb8013], 0xf
