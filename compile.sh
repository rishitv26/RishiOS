/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/kernel.cpp" -o "bin/kernel.o"
nasm "kernel.asm" -f elf -o "bin/kernel_entry.o"
/usr/local/i386elfgcc/bin/i386-elf-ld -o "bin/full_kernel.bin" -Ttext 0x1000 "bin/kernel_entry.o" "bin/kernel.o" --oformat binary
nasm "boot.asm" -f bin -o "bin/boot.bin"
cat "bin/boot.bin" "bin/full_kernel.bin" > "bin/e.bin"
nasm "zeros.asm" -f bin -o "bin/zero.bin"
cat "bin/e.bin" "bin/zero.bin" > "bin/os.bin"
qemu-system-x86_64 bin/os.bin
