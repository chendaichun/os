TOOL_PREFIX = x86_64-elf-
CFLAGS = -g -c -O0 -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc -I./inlcude 

boot:
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.S -o ./build/boot.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.c -o ./build/bootc.o
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x7c00 ./build/boot.o ./build/bootc.o -o ./build/boot_all.elf
	${TOOL_PREFIX}objcopy -O binary ./build/boot_all.elf ./build/boot_all.bin
	${TOOL_PREFIX}objdump -d --section=.text ./build/boot_all.elf > ./asm/boot.txt
