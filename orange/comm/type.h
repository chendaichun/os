#ifndef TYPES
#define TYPES

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// 内存管理相关类型
typedef uint32_t pte_t;	 // 页表项类型
typedef uint32_t pde_t;	 // 页目录项类型
typedef uint32_t size_t; // 大小类型

#endif