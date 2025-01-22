#include "../../../comm/bootinfo.h"
#include "../include/console.h"
#include "../include/mm/memory.h"
void kernel_init(boot_info_t *boot_info) {
	// 初始化内核打印功能
	console_init();
	console_write("Kernel initialized!\n");
	console_write_color("Hello OS World!\n", VGA_LIGHT_GREEN, VGA_BLACK);

	mm_init(boot_info);

	while (1)
		;
}