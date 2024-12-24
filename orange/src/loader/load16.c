
__asm__(".code16gcc");
#include "loader.h"

boot_info_t boot_info;

static void show_msg(const char *msg) {
  char c;
  // 使用bios写显存，持续往下写
  while ((c = *msg++) != '\0') {
    __asm__ __volatile__("mov $0xe, %%ah\n\t"
                         "mov %[ch], %%al\n\t"
                         "int $0x10" ::[ch] "r"(c));
  }
}

static void  detect_memory(void) {
	uint32_t contID = 0;
	SMAP_entry_t smap_entry;
	int signature, bytes;
    show_msg("try to detect memory:");
	// 初次：EDX=0x534D4150,EAX=0xE820,ECX=24,INT 0x15, EBX=0（初次）
	// 后续：EAX=0xE820,ECX=24,
	// 结束判断：EBX=0
	boot_info.ram_region_count = 0;
	for (int i = 0; i < BOOT_RAM_REGION_MAX; i++) {
		SMAP_entry_t * entry = &smap_entry;
		__asm__ __volatile__("int  $0x15"
			: "=a"(signature), "=c"(bytes), "=b"(contID)
			: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(entry));
		if (signature != 0x534D4150) {
            show_msg("failed.\r\n");
			return;
		}
		// todo: 20字节
		if (bytes > 20 && (entry->ACPI & 0x0001) == 0){
			continue;
		}
        // 保存RAM信息，只取32位，空间有限无需考虑更大容量的情况
        if (entry->Type == 1) {
            boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;
            boot_info.ram_region_cfg[boot_info.ram_region_count].size = entry->LengthL;
            boot_info.ram_region_count++;
        }
		if (contID == 0) {
			break;
		}
	}
    show_msg("ok.\r\n");
}



uint16_t gdt_table[][4] = {
    {0, 0, 0, 0},
    {0xFFFF, 0x0000, 0x9A00, 0x00CF},
    {0xFFFF, 0x0000, 0x9200, 0x00CF},
};
extern struct {
		uint16_t limit;
		uint16_t start15_0;
		uint16_t start31_16;
} gdt = {
	sizeof(gdt_table) - 1,0x0000, 0x0000
};

void start_load() {
    //show_msg("\n");
    show_msg("\010loading...");
    detect_memory();
	gdt.start15_0 = (uint32_t)gdt_table & 0xffff;
	gdt.start31_16 = (uint32_t)gdt_table >> 16;
	((void (*)(void))0x8005)();
	while (1);
}