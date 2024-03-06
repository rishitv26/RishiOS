cd ../user # compile all user items

# generate user library:
nasm -f elf64 -o ../bin/bin/user/syscall.o usrlib/syscall.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c usrlib/stdio.c -o ../bin/bin/user/stdio.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c usrlib/stdstr.c -o ../bin/bin/user/stdstr.o &&
# assemble library into static file:
ar -rcs ../bin/lib/usrlib.a\
    ../bin/bin/user/syscall.o\
    ../bin/bin/user/stdio.o\
    ../bin/bin/user/stdstr.o &&

# compile rest of the files:
nasm -f elf64 -o ../bin/bin/user/start.o start.asm &&
nasm -f elf64 -o ../bin/bin/user/start2.o start2.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main.c -o ../bin/bin/user/main.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main2.c -o ../bin/bin/user/main2.o &&
cd ..

# link files together and generate binaries:
ld -nostdlib -T scripts/link_user.lds -o bin/bin/user/usr\
    bin/bin/user/start.o\
    bin/bin/user/main.o\
    bin/lib/usrlib.a &&
objcopy -O binary bin/bin/user/usr bin/bin/user.bin &&

ld -nostdlib -T scripts/link_user.lds -o bin/bin/user/usr2\
    bin/bin/user/start2.o\
    bin/bin/user/main2.o\
    bin/lib/usrlib.a &&
objcopy -O binary bin/bin/user/usr2 bin/bin/user2.bin

cd scripts # done...