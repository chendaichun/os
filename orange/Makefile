all : build	
	dd if=/dev/zero of=./build/disk.img bs=1024 count=1000
	dd if=./build/boot_all.bin of=./build/disk.img conv=notrunc
	dd if=./build/load_all.bin of=./build/disk.img bs=512 seek=2 conv=notrunc
	dd if=./build/kernel_init.bin of=./build/disk.img bs=512 seek=100 conv=notrunc

build :	
	mkdir -p build
	mkdir -p asm
	
	make -f ./src/boot/makefile Makefile boot
	make -f ./src/loader/makefile Makefile loader
	make -f ./src/kernel/makefile Makefile kernel

start : all
	qemu-system-i386  -S -gdb tcp::1234 -m 128M  -drive file=./build/disk.img,index=0,media=disk,format=raw

clear :
	rm -rf build