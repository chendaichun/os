TOOL_PREFIX = x86_64-elf-
CFLAGS = -g -c -O0 -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc -I./inlcude 
all :
	mkdir -p build
	mkdir -p asm

	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.S -o ./build/boot.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.c -o ./build/bootc.o
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x7c00 ./build/boot.o ./build/bootc.o -o ./build/boot_all.elf
	${TOOL_PREFIX}objcopy -O binary ./build/boot_all.elf ./build/boot_all.bin
	${TOOL_PREFIX}objdump -d --section=.text ./build/boot_all.elf > ./asm/boot.txt

	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/loader.S -o ./build/load.o 
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/load16.c -o ./build/load16.o 
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/load32.c -o ./build/load32.o 
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x8000 ./build/load.o ./build/load16.o ./build/load32.o -o ./build/load_all.elf
	${TOOL_PREFIX}objdump -d --section=.text ./build/load_all.elf > ./asm/load.txt

	${TOOL_PREFIX}objcopy -O binary ./build/load_all.elf ./build/load_all.bin
	
	dd if=/dev/zero of=./build/disk.img bs=1024 count=1000
	dd if=./build/boot_all.bin of=./build/disk.img conv=notrunc
	dd if=./build/load_all.bin of=./build/disk.img bs=512 seek=2 conv=notrunc
	qemu-system-i386  -S -gdb tcp::1234 -m 128M  -drive file=./build/disk.img,index=0,media=disk,format=raw

clear :
	rm -rf build