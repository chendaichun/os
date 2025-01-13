include makefile.head
boot:
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.S -o ./build/boot.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/boot/boot.c -o ./build/bootc.o
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x7c00 ./build/boot.o ./build/bootc.o -o ./build/boot_all.elf
	${TOOL_PREFIX}objcopy -O binary ./build/boot_all.elf ./build/boot_all.bin
	${TOOL_PREFIX}objdump -d --section=.text ./build/boot_all.elf > ./asm/boot.txt
