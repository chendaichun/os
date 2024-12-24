#ifndef LOADER_H
#define LOADER_H

#include "../../comm/type.h"

#define BOOT_RAM_REGION_MAX			10		// RAM区最大数量

typedef struct _boot_info_t {
    // RAM区信息
    struct {
        uint32_t start;
        uint32_t size;
    }ram_region_cfg[BOOT_RAM_REGION_MAX];
    int ram_region_count;
}boot_info_t;

// 内存检测信息结构
typedef struct SMAP_entry {
    uint32_t BaseL; // base address uint64_t
    uint32_t BaseH;
    uint32_t LengthL; // length uint64_t
    uint32_t LengthH;
    uint32_t Type; // entry Type
    uint32_t ACPI; // extended
}__attribute__((packed)) SMAP_entry_t;

extern boot_info_t boot_info;

#endif