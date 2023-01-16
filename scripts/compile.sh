cd .. # get out of scripts folder

cd bootloader # start with compiling bootloader...
nasm boot.asm -f bin -o ../bin/bin/boot.bin
cd ..

cd kernel # kernel time!
nasm kernel.asm -f bin -o ../bin/bin/kernel.bin
cd ..

# time to give birth to an operating system!
cat "bin/bin/boot.bin" "bin/bin/kernel.bin" > "bin/iso/os.bin"

cd scripts # end of compilation