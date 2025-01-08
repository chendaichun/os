.code16
.global _start
.extern start_load
.extern gdt
_start:
    jmp start_load
    nop
    nop
    cli
    mov %cr0, %eax
	orl $1, %eax
	mov %eax, %cr0
    lgdt [gdt]
    jmp $8, $protect_mode_entry

.code32
.extern load_kernel
protect_mode_entry:
    mov $16, %ax		// 16为数据段选择子
	mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    jmp $8, $load_kernel