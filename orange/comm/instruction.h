#ifndef INSTRCUT
#define INSTRCUT
#include "type.h"

static inline void outb(uint16_t port, uint8_t data) {
	__asm__ __volatile__("outb %[v], %[p]" : : [p] "d"(port), [v] "a"(data));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t rv;
	__asm__ __volatile__("in %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}

static inline uint8_t inb(uint16_t port) {
	uint8_t rv;
	__asm__ __volatile__("inb %[p], %[v]" : [v] "=a"(rv) : [p] "d"(port));
	return rv;
}

static uint32_t get_cr3(void) {
	uint32_t cr3;
	__asm__ __volatile__("movl %%cr3, %0" : "=r"(cr3));
	return cr3;
}

// CR3寄存器操作
static void load_cr3(uint32_t pde) {
	__asm__ __volatile__("movl %0, %%cr3" ::"r"(pde));
}
#endif