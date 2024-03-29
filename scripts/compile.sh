sh compile_user.sh &&
sh compile_loader.sh &&
cd .. &&

# cd kernel # compile kernel
# nasm -f elf64 -o ../bin/bin/kernel_entry.o main.asm &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main.c -o ../bin/bin/ker.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c interrupt/inter.c -o ../bin/bin/int.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c lib/strlen.c -o ../bin/bin/strlen.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c mem/mem.c -o ../bin/bin/freemem.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c proccess/proccess.c -o ../bin/bin/proccess.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c syscalls/syscall.c -o ../bin/bin/syscall.o &&
# gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c keyboard/keyboard.c -o ../bin/bin/keyboard.o &&
# nasm -f elf64 -o ../bin/bin/lib.o lib/lib.asm &&
# nasm -f elf64 -o ../bin/bin/inta.o interrupt/trap.asm &&
# cd ..

# # link elf binaries together
# ld -nostdlib -T scripts/link_kernel.lds -o bin/bin/kernel\
#     bin/bin/kernel_entry.o\
#     bin/bin/ker.o\
#     bin/bin/int.o\
#     bin/bin/inta.o\
#     bin/bin/strlen.o\
#     bin/bin/lib.o\
#     bin/bin/freemem.o\
#     bin/bin/proccess.o\
#     bin/bin/syscall.o\
#     bin/bin/keyboard.o &&
# objcopy -O binary bin/bin/kernel bin/bin/kernel.bin &&

# time to give birth to an operating system!
dd if=bin/bin/boot.bin of=os.img bs=512 count=1 conv=notrunc && # slap in the bootloader...
dd if=bin/bin/loader.bin of=os.img bs=512 count=15 seek=1 conv=notrunc && # slap in the loader...
# dd if=bin/bin/kernel.bin of=rishios.img bs=512 count=100 seek=6 conv=notrunc && # slap in the others...
# dd if=bin/bin/gc.bin of=rishios.img bs=512 count=10 seek=106 conv=notrunc && # slap in usr1...
# dd if=bin/bin/np.bin of=rishios.img bs=512 count=10 seek=116 conv=notrunc && # slap in usr2...
# dd if=bin/bin/default.bin of=rishios.img bs=512 count=10 seek=126 conv=notrunc && # slap in usr2...
echo COMPILATION WAS SUCCESSFULL! &&

cd scripts # end of compilation