sh compile_user.sh &&
cd .. &&

cd bootloader && # compile bootloader
nasm -f bin -o ../bin/bin/boot.bin boot.asm &&
nasm -f bin -o ../bin/bin/loader.bin loader.asm &&
cd .. # done with bootloader

cd kernel # compile kernel
nasm -f elf64 -o ../bin/bin/kernel_entry.o main.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main.c -o ../bin/bin/ker.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c interrupt/inter.c -o ../bin/bin/int.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c lib/strlen.c -o ../bin/bin/strlen.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c mem/mem.c -o ../bin/bin/freemem.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c proccess/proccess.c -o ../bin/bin/proccess.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c lib/syscall.c -o ../bin/bin/syscall.o &&
nasm -f elf64 -o ../bin/bin/lib.o lib/lib.asm &&
nasm -f elf64 -o ../bin/bin/inta.o interrupt/trap.asm &&
cd ..

# link elf binaries together
ld -nostdlib -T scripts/link_kernel.lds -o bin/bin/kernel\
    bin/bin/kernel_entry.o\
    bin/bin/ker.o\
    bin/bin/int.o\
    bin/bin/inta.o\
    bin/bin/strlen.o\
    bin/bin/lib.o\
    bin/bin/freemem.o\
    bin/bin/proccess.o\
    bin/bin/syscall.o &&
objcopy -O binary bin/bin/kernel bin/bin/kernel.bin &&

# time to give birth to an operating system!
dd if=bin/bin/boot.bin of=boot.img bs=512 count=1 conv=notrunc && # slap in the bootloader...
dd if=bin/bin/loader.bin of=boot.img bs=512 count=5 seek=1 conv=notrunc && # slap in the loader...
dd if=bin/bin/kernel.bin of=boot.img bs=512 count=100 seek=6 conv=notrunc && # slap in the others...
dd if=bin/bin/user.bin of=boot.img bs=512 count=10 seek=106 conv=notrunc && # slap in the others...
clear &&
echo COMPILATION WAS SUCCESSFULL! &&

cd scripts # end of compilation