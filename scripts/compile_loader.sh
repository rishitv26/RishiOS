cd ../bootloader

# compile all items in bootloader.
nasm -f bin -o ../bin/bin/boot.bin boot.asm &&
nasm -f bin -o ../bin/bin/loader.bin loader.asm &&
nasm -f elf64 -o ../bin/bin/entry.o cmodule/entry.asm &&
nasm -f elf64 -o ../bin/bin/libasm.o cmodule/lib.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c cmodule/emain.c -o ../bin/bin/emain.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c cmodule/lib.c -o ../bin/bin/lib.o &&
ld -nostdlib -T ../scripts/link_loader.lds -o ../bin/bin/enter\
    ../bin/bin/entry.o\
    ../bin/bin/emain.o\
    ../bin/bin/lib.o\
    ../bin/bin/libasm.o &&
objcopy -O binary ../bin/bin/enter ../bin/bin/enter.bin &&
dd if=../bin/bin/enter.bin >> ../bin/bin/loader.bin &&

cd ../scripts
