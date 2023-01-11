cd ..
# compilation of files:
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c "kernel/kernel.cpp" -o "bin/kernel.o"
nasm "bootloader/kernel.asm" -f elf -o "bin/kernel_entry.o"
nasm "bootloader/zeros.asm" -f bin -o "bin/zero.bin"
nasm "bootloader/boot.asm" -f bin -o "bin/boot.bin"

# linking:
/usr/local/i386elfgcc/bin/i386-elf-ld -o "bin/full_kernel.bin" -Ttext 0x1000\
 "bin/kernel_entry.o"\
  "bin/kernel.o"\
   --oformat binary
cat "bin/boot.bin" "bin/full_kernel.bin" > "bin/e.bin"
cat "bin/e.bin" "bin/zero.bin" > "bin/os.bin"

# end of compilation
cd buildscripts
