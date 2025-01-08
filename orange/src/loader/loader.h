#ifndef LOADER_H
#define LOADER_H

#include "../../comm/bootinfo.h"
#include "../../comm/instruction.h"
#include "../../comm/type.h"

// 内存检测信息结构
typedef struct SMAP_entry {
	uint32_t BaseL; // base address uint64_t
	uint32_t BaseH;
	uint32_t LengthL; // length uint64_t
	uint32_t LengthH;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended
} __attribute__((packed)) SMAP_entry_t;

extern boot_info_t boot_info;
// 以上全是和内存检测相关的
#define KERNEL_ADDR (1024 * 1024)
#endif