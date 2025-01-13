#ifndef CONSOLE_H
#define CONSOLE_H

#include "../../../comm/type.h"
#include "../../../comm/instruction.h"

// 定义显存的起始地址
#define VIDEO_MEMORY_ADDR 0xB8000
// 定义屏幕的宽度和高度（标准VGA文本模式为80x25）
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

// 颜色定义
enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15,
};

void console_init(void);
void console_put_char(char c);
void console_write(const char* str);
void console_write_color(const char* str, uint8_t forecolor, uint8_t backcolor);
void console_clear(void);

#endif