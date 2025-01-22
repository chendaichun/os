include makefile.head
kernel:
	@echo "kernel build start"
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/init/start.S -o ./build/kernel_init.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/init/start.c -o ./build/kernel_init_c.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/init/console.c -o ./build/console_c.o

	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/mm/memory.c -o ./build/memory_c.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/mm/mm.c -o ./build/pmm_c.o

	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x100000 ./build/kernel_init.o ./build/kernel_init_c.o ./build/console_c.o ./build/memory_c.o ./build/pmm_c.o -o ./build/kernel_init.elf
	${TOOL_PREFIX}objdump -d --section=.text ./build/kernel_init.elf > ./asm/kernel.txt
	${TOOL_PREFIX}objcopy -O binary ./build/kernel_init.elf ./build/kernel_init.bin
	@echo "kernel build successsed"