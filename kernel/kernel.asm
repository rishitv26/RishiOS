mov bx, message
call print
jmp $

%include 'print_str.asm'

message: db "Welcome to the Kernel!", 0