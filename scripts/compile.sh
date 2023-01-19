cd .. # get out of scripts folder

cd bootloader # compile bootloader
nasm -f bin -o ../bin/bin/boot.bin boot.asm
nasm -f bin -o ../bin/bin/loader.bin loader.asm
cd .. # done with bootloader

cd kernel # compile kernel
nasm -f bin -o ../bin/bin/kernel_entry.bin main.asm
cd ..

# time to give birth to an operating system!
dd if=bin/bin/boot.bin of=boot.img bs=512 count=1 conv=notrunc # slap in the bootloader...
dd if=bin/bin/loader.bin of=boot.img bs=512 count=5 seek=1 conv=notrunc # slap in the loader...
dd if=bin/bin/kernel_entry.bin of=boot.img bs=512 count=100 seek=6 conv=notrunc # slap in the others...

cd scripts # end of compilation