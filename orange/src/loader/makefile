include makefile.head
loader:
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/loader.S -o ./build/load.o 
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/load16.c -o ./build/load16.o 
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/loader/load32.c -o ./build/load32.o 
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x8000 ./build/load.o ./build/load16.o ./build/load32.o -o ./build/load_all.elf
	${TOOL_PREFIX}objdump -d --section=.text ./build/load_all.elf > ./asm/load.txt
	${TOOL_PREFIX}objcopy -O binary ./build/load_all.elf ./build/load_all.bin