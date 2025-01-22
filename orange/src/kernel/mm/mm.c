#include "../include/mm/memory.h"
memory_manager_t mm;
// 位图操作函数
static void bitmap_set(uint32_t idx) {
	uint32_t byte_idx = idx / 8;
	uint32_t bit_idx = idx % 8;
	mm.bitmap[byte_idx] |= (1 << bit_idx);
}

static void bitmap_clear(uint32_t idx) {
	uint32_t byte_idx = idx / 8;
	uint32_t bit_idx = idx % 8;
	mm.bitmap[byte_idx] &= ~(1 << bit_idx);
}

static int bitmap_test(uint32_t idx) {
	uint32_t byte_idx = idx / 8;
	uint32_t bit_idx = idx % 8;
	return (mm.bitmap[byte_idx] & (1 << bit_idx)) != 0;
}

// 初始化物理内存管理
extern void pmm_init(boot_info_t *boot_info) {
	// 找到最大的可用内存地址
	uint32_t max_mem = 0;
	for (int i = 0; i < boot_info->ram_region_count; i++) {
		uint32_t region_end = boot_info->ram_region_cfg[i].start +
							  boot_info->ram_region_cfg[i].size;
		if (region_end > max_mem) {
			max_mem = region_end;
		}
	}

	// 计算总页数
	mm.total_pages = max_mem / PAGE_SIZE;
	mm.free_pages = mm.total_pages;

	// 计算位图大小并分配空间
	mm.bitmap_length = (mm.total_pages + 7) / 8;
	// 将位图放在1MB位置之后
	mm.bitmap = (uint8_t *)0x100000;

	// 初始化位图，先标记所有页面为已使用
	for (uint32_t i = 0; i < mm.bitmap_length; i++) {
		mm.bitmap[i] = 0xFF;
	}

	// 根据内存布局信息，标记可用区域
	for (int i = 0; i < boot_info->ram_region_count; i++) {
		uint32_t start_page = boot_info->ram_region_cfg[i].start / PAGE_SIZE;
		uint32_t pages = boot_info->ram_region_cfg[i].size / PAGE_SIZE;

		for (uint32_t page = start_page; page < start_page + pages; page++) {
			bitmap_clear(page);
		}
	}

	// 标记内核和位图占用的内存
	uint32_t kernel_start_page = 0x100000 / PAGE_SIZE;
	uint32_t bitmap_pages = (mm.bitmap_length + PAGE_SIZE - 1) / PAGE_SIZE;

	for (uint32_t page = kernel_start_page;
		 page < kernel_start_page + bitmap_pages; page++) {
		bitmap_set(page);
		mm.free_pages--;
	}
}

// 页目录和页表项结构
typedef uint32_t pde_t;
typedef uint32_t pte_t;
// 开启分页
static void enable_paging(void) {
	uint32_t cr0;
	__asm__ __volatile__("movl %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("movl %0, %%cr0" ::"r"(cr0));
}

// 初始化虚拟内存管理
extern void vmm_init(void) {
	// 分配页目录
	mm.kernel_page_dir = alloc_page();
	pde_t *pde = (pde_t *)mm.kernel_page_dir;

	// 清空页目录
	for (int i = 0; i < 1024; i++) {
		pde[i] = 0;
	}

	// 映射前4MB的内存（内核空间）
	for (uint32_t addr = 0; addr < 0x400000; addr += PAGE_SIZE) {
		map_page(addr, addr, PTE_P | PTE_W);
	}

	// 加载页目录并启用分页
	load_cr3(mm.kernel_page_dir);
	enable_paging();
}

// 在位图中查找第一个空闲页
static uint32_t find_first_free_page(void) {
	for (uint32_t i = 0; i < mm.bitmap_length * 8; i++) {
		uint32_t byte_idx = i / 8;
		uint32_t bit_idx = i % 8;
		if (!(mm.bitmap[byte_idx] & (1 << bit_idx))) {
			return i;
		}
	}
	return 0xFFFFFFFF; // 没有空闲页
}

// 分配一个物理页面
uint32_t alloc_page(void) {
	if (mm.free_pages == 0) {
		return 0; // 内存耗尽
	}

	// 查找第一个空闲页
	uint32_t page_idx = find_first_free_page();
	if (page_idx == 0xFFFFFFFF) {
		return 0;
	}

	// 标记页面为已使用
	uint32_t byte_idx = page_idx / 8;
	uint32_t bit_idx = page_idx % 8;
	mm.bitmap[byte_idx] |= (1 << bit_idx);
	mm.free_pages--;

	// 返回物理地址
	return page_idx * PAGE_SIZE;
}

// 释放一个物理页面
void free_page(uint32_t page_addr) {
	// 确保地址按页对齐
	if (page_addr & (PAGE_SIZE - 1)) {
		return;
	}

	uint32_t page_idx = page_addr / PAGE_SIZE;

	// 检查页面索引是否有效
	if (page_idx >= mm.total_pages) {
		return;
	}

	// 检查页面是否已经被释放
	uint32_t byte_idx = page_idx / 8;
	uint32_t bit_idx = page_idx % 8;
	if (!(mm.bitmap[byte_idx] & (1 << bit_idx))) {
		return; // 页面已经是空闲的
	}

	// 标记页面为空闲
	mm.bitmap[byte_idx] &= ~(1 << bit_idx);
	mm.free_pages++;
}

// 简单的内存块头部结构
typedef struct block_header {
	uint32_t size;			   // 块大小（不包括头部）
	uint8_t is_free;		   // 是否空闲
	struct block_header *next; // 下一个块
} block_header_t;

static block_header_t *heap_start = NULL;

// 初始化堆
static void init_heap(void) {
	if (heap_start != NULL)
		return;

	// 分配初始堆空间（例如4页）
	uint32_t heap_addr = alloc_page();
	if (heap_addr == 0)
		return;

	heap_start = (block_header_t *)heap_addr;
	heap_start->size = PAGE_SIZE - sizeof(block_header_t);
	heap_start->is_free = 1;
	heap_start->next = NULL;
}

// 分配指定大小的内存
void *kmalloc(uint32_t size) {
	if (size == 0)
		return NULL;

	// 确保堆已初始化
	if (heap_start == NULL) {
		init_heap();
		if (heap_start == NULL)
			return NULL;
	}

	// 对齐size到4字节
	size = (size + 3) & ~3;

	// 查找合适的空闲块
	block_header_t *current = heap_start;
	block_header_t *best_fit = NULL;
	uint32_t smallest_size = 0xFFFFFFFF;

	while (current != NULL) {
		if (current->is_free && current->size >= size) {
			if (current->size < smallest_size) {
				best_fit = current;
				smallest_size = current->size;
			}
		}
		current = current->next;
	}

	// 如果没找到合适的块，尝试分配新页
	if (best_fit == NULL) {
		uint32_t pages_needed =
			(size + sizeof(block_header_t) + PAGE_SIZE - 1) / PAGE_SIZE;
		uint32_t new_area =
			alloc_page(); // 这里简化为只分配一页，实际应该分配pages_needed页
		if (new_area == 0)
			return NULL;

		block_header_t *new_block = (block_header_t *)new_area;
		new_block->size = PAGE_SIZE - sizeof(block_header_t);
		new_block->is_free = 1;

		// 将新块添加到链表
		new_block->next = heap_start;
		heap_start = new_block;
		best_fit = new_block;
	}

	// 如果块太大，需要分割
	if (best_fit->size >= size + sizeof(block_header_t) + 4) {
		block_header_t *new_block =
			(block_header_t *)((uint8_t *)best_fit + sizeof(block_header_t) +
							   size);
		new_block->size = best_fit->size - size - sizeof(block_header_t);
		new_block->is_free = 1;
		new_block->next = best_fit->next;

		best_fit->size = size;
		best_fit->next = new_block;
	}

	best_fit->is_free = 0;
	return (void *)((uint8_t *)best_fit + sizeof(block_header_t));
}

// 释放内存
void kfree(void *ptr) {
	if (ptr == NULL)
		return;

	// 获取块头部
	block_header_t *header =
		(block_header_t *)((uint8_t *)ptr - sizeof(block_header_t));

	// 基本验证
	if ((uint32_t)header < (uint32_t)heap_start)
		return;

	// 标记为空闲
	header->is_free = 1;

	// 合并相邻的空闲块
	block_header_t *current = heap_start;
	while (current != NULL && current->next != NULL) {
		if (current->is_free && current->next->is_free) {
			current->size += sizeof(block_header_t) + current->next->size;
			current->next = current->next->next;
		} else {
			current = current->next;
		}
	}
}