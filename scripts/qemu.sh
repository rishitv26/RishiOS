cd ..
sudo qemu-system-x86_64 -m 1G -smp 1 -cpu host -drive file=boot.img,format=raw -enable-kvm -mem-path /dev/hugepages
cd scripts