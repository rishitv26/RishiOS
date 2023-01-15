[GLOBAL idt_flush] ; get idt_flush function from c code

idt_flush:
    mov eax, [esp + 4] ; get idt pointer
    lidt [eax]         ; load pointer
    ret                ; return void

[EXTERN isr_handler]

isr_common_stub: ; common interrupt handler
   pusha                    ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

   mov ax, ds               ; lower 16-bits of eax, set to ds
   push eax                 ; save data segment descriptor

   mov ax, 0x10  ; load kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call isr_handler

   pop eax        ; reload original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; removes edi, esi, ebp
   add esp, 8     ; cleans up the errors
   sti
   iret           ; cleans up the errors