cd ../user # compile all user items

# generate user library:
nasm -f elf64 -o ../bin/bin/user/syscall.o usrlib/syscall.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c usrlib/stdio.c -o ../bin/bin/user/stdio.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c usrlib/stdstr.c -o ../bin/bin/user/stdstr.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c usrlib/lib.c -o ../bin/bin/user/lib.o &&
# assemble library into static file:
ar -rcs ../bin/lib/usrlib.a\
    ../bin/bin/user/syscall.o\
    ../bin/bin/user/stdio.o\
    ../bin/bin/user/stdstr.o\
    ../bin/bin/user/lib.o &&

# compile rest of the files:
nasm -f elf64 -o ../bin/bin/user/collector_asm.o garbageCollector/collector.asm &&
nasm -f elf64 -o ../bin/bin/user/nullproc_asm.o nullProccess/np.asm &&
nasm -f elf64 -o ../bin/bin/user/default_asm.o default.asm &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c garbageCollector/collector.c -o ../bin/bin/user/collector.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c nullProccess/np.c -o ../bin/bin/user/nullproc.o &&
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c default.c -o ../bin/bin/user/default.o &&
cd ..

# link files together and generate binaries:
ld -nostdlib -T scripts/link_user.lds -o bin/bin/user/gc\
    bin/bin/user/collector_asm.o\
    bin/bin/user/collector.o\
    bin/lib/usrlib.a &&
objcopy -O binary bin/bin/user/gc bin/bin/gc.bin &&

ld -nostdlib -T scripts/link_user.lds -o bin/bin/user/np\
    bin/bin/user/nullproc_asm.o\
    bin/bin/user/nullproc.o\
    bin/lib/usrlib.a &&
objcopy -O binary bin/bin/user/np bin/bin/np.bin &&

ld -nostdlib -T scripts/link_user.lds -o bin/bin/user/default\
    bin/bin/user/default_asm.o\
    bin/bin/user/default.o\
    bin/lib/usrlib.a &&
objcopy -O binary bin/bin/user/default bin/bin/default.bin &&

cd scripts # done...