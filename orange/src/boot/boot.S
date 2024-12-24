
.code16
.global _start
.extern entry_loader
_start:
    // 重置数据段寄存器
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
    // 设置栈
    mov $0x7c00, %sp
    // 显示加载完成
    mov $0xe, %ah
	mov $'B', %al
	int $0x10
    
    // read_loader
read_loader:
	mov $0x7e00, %bx	// 读取到的内存地址
	mov $0x2, %cx		// ch:磁道号，cl起始扇区号
	mov $0x2, %ah		// ah: 0x2读磁盘命令
	mov $0x63, %al		// al: 读取的扇区数量
	mov $0x0080, %dx	// dh: 磁头号，dl驱动器号0x80(磁盘1)
	int $0x13
	jc read_loader

    jmp entry_loader


    .org 0x1fe
    .word 0xaa55
