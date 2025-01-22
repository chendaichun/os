#include "../include/mm/memory.h"

// 外部函数声明
extern void pmm_init(boot_info_t *boot_info);
extern void vmm_init(void);

void mm_init(boot_info_t *boot_info) {
	// 初始化物理内存管理
	pmm_init(boot_info);

	// 初始化虚拟内存管理（分页）
	vmm_init();

	// 完成初始化后可以输出一些信息
	// printf("Memory initialized: %d pages total, %d pages free\n",
	//     mm.total_pages, mm.free_pages);
}

// 映射虚拟地址到物理地址
int map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
	uint32_t pd_idx = vaddr >> 22;
	uint32_t pt_idx = (vaddr >> 12) & 0x3FF;

	pde_t *pde = (pde_t *)get_cr3();
	pte_t *pte;

	// 如果页表不存在，创建它
	if (!(pde[pd_idx] & PTE_P)) {
		uint32_t pt = alloc_page();
		if (!pt)
			return -1;

		pde[pd_idx] = pt | PTE_P | PTE_W | PTE_U;
		pte = (pte_t *)(pt & PAGE_MASK);

		// 清空新页表
		for (int i = 0; i < 1024; i++) {
			pte[i] = 0;
		}
	} else {
		pte = (pte_t *)(pde[pd_idx] & PAGE_MASK);
	}

	// 设置页表项
	pte[pt_idx] = (paddr & PAGE_MASK) | flags;

	// 刷新TLB
	__asm__ __volatile__("invlpg (%0)" ::"r"(vaddr) : "memory");

	return 0;
}