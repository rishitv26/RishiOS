cd ..
# compilation of files:
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/kernel.c" -o "bin/kernel.o"
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/common.c" -o "bin/common.o"
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/screen/monitor.c" -o "bin/monitor.o"
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/interrupts/des_tables.c" -o "bin/idt.o"
nasm "bootloader/kernel.asm" -f elf -o "bin/kernel_entry.o"
nasm "bootloader/zeros.asm" -f bin -o "bin/zero.bin"
nasm "bootloader/boot.asm" -f bin -o "bin/boot.bin"
nasm "kernel/interrupts/interrupt.asm" -f bin -o "bin/inter.bin"
nasm "kernel/interrupts/idt.asm" -f bin -o "bin/idt.bin"

# linking:
/usr/local/i386elfgcc/bin/i386-elf-ld -o "bin/full_kernel.bin" -Ttext 0x1000\
 "bin/kernel_entry.o"\
  "bin/kernel.o"\
  "bin/common.o"\
  "bin/monitor.o"\
  "bin/idt.o"\
   --oformat binary
cat "bin/inter.bin" "bin/idt.bin" "bin/boot.bin" "bin/full_kernel.bin" > "bin/e.bin"
cat "bin/e.bin" "bin/zero.bin" > "bin/os.bin"

# end of compilation
cd buildscripts
