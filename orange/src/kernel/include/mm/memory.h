#ifndef MEMORY_H
#define MEMORY_H

#include "../../../../comm/bootinfo.h"
#include "../../../../comm/instruction.h"
#include "../../../../comm/type.h"
// 页大小定义 (4KB)
#define PAGE_SIZE 4096
#define PAGE_SIZE_BITS 12

// 页掩码，用于地址对齐
#define PAGE_MASK 0xFFFFF000

// 基本定义
#ifndef NULL
#define NULL ((void *)0)
#endif

// 内存管理器状态
typedef struct memory_manager {
	uint32_t total_pages;	  // 总的物理页面数
	uint32_t free_pages;	  // 空闲页面数
	uint8_t *bitmap;		  // 位图，用于记录页面使用情况
	uint32_t bitmap_length;	  // 位图长度（字节）
	uint32_t kernel_page_dir; // 内核页目录物理地址
} memory_manager_t;

// 页表项标志
#define PTE_P 1		// Present
#define PTE_W 2		// Writeable
#define PTE_U 4		// User
#define PTE_PS 0x80 // Page size extension

// 函数声明
void mm_init(boot_info_t *boot_info);
uint32_t alloc_page(void);
void free_page(uint32_t page_addr);
void *kmalloc(uint32_t size);
void kfree(void *ptr);

// 虚拟内存映射函数
int map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags);
void unmap_page(uint32_t vaddr);

#endif