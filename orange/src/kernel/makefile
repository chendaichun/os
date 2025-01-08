TOOL_PREFIX = x86_64-elf-
CFLAGS = -g -c -O0 -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc -I./inlcude 

kernel:
	@echo "kernel build start"
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/init/start.S -o ./build/kernel_init.o
	$(TOOL_PREFIX)gcc $(CFLAGS) ./src/kernel/init/start.c -o ./build/kernel_init_c.o
	$(TOOL_PREFIX)ld -m elf_i386 -Ttext=0x100000 ./build/kernel_init.o ./build/kernel_init_c.o -o ./build/kernel_init.elf
	${TOOL_PREFIX}objdump -d --section=.text ./build/kernel_init.elf > ./asm/kernel.txt
	${TOOL_PREFIX}objcopy -O binary ./build/kernel_init.elf ./build/kernel_init.bin
	@echo "kernel build successsed"