#include "../include/console.h"

// 屏幕光标位置
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

// 当前显示字符的颜色
static uint8_t console_forecolor = VGA_WHITE;
static uint8_t console_backcolor = VGA_BLACK;

// 显存指针
static uint16_t* video_memory = (uint16_t*)VIDEO_MEMORY_ADDR;

// 更新硬件光标位置
static void update_cursor(void) {
    uint16_t cursor_location = cursor_y * CONSOLE_WIDTH + cursor_x;
    
    // 通过端口操作更新显卡的光标位置
    outb(0x3D4, 14);                    // 告诉VGA我们要设置光标的高字节
    outb(0x3D5, cursor_location >> 8);  // 发送高8位
    outb(0x3D4, 15);                    // 告诉VGA我们要设置光标的低字节
    outb(0x3D5, cursor_location);       // 发送低8位
}

// 滚动屏幕
static void scroll(void) {
    // 属性字节
    uint8_t attribute_byte = (console_backcolor << 4) | (console_forecolor & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);  // 空格符及其属性

    // 如果光标超出屏幕底部，需要滚动
    if(cursor_y >= CONSOLE_HEIGHT) {
        // 将所有行上移一行
        for (int i = 0; i < (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH; i++) {
            video_memory[i] = video_memory[i + CONSOLE_WIDTH];
        }

        // 最后一行填充空格
        for (int i = (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++) {
            video_memory[i] = blank;
        }

        cursor_y = CONSOLE_HEIGHT-1;
    }
}

extern void console_init(void) {
    // 初始化光标位置
    cursor_x = 0;
    cursor_y = 0;
    
    // 清屏
    console_clear();
    
    // 更新光标位置
    update_cursor();
}

void console_clear(void) {
    uint8_t attribute_byte = (console_backcolor << 4) | (console_forecolor & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    // 填充整个屏幕
    for(int i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++) {
        video_memory[i] = blank;
    }

    // 重置光标位置
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void console_put_char(char c) {
    uint8_t attribute_byte = (console_backcolor << 4) | (console_forecolor & 0x0F);
    uint16_t attribute = attribute_byte << 8;
    uint16_t* location;

    if (c == 0x08 && cursor_x) { // 退格键
        cursor_x--;
    } else if (c == '\r') {  // 回车键
        cursor_x = 0;
    } else if (c == '\n') {  // 换行键
        cursor_x = 0;
        cursor_y++;
    } else if (c >= ' ') {   // 可打印字符
        location = video_memory + (cursor_y * CONSOLE_WIDTH + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // 处理行末
    if (cursor_x >= CONSOLE_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // 滚动屏幕（如果需要）
    scroll();
    // 移动硬件光标
    update_cursor();
}

void console_write(const char* str) {
    while (*str) {
        console_put_char(*str++);
    }
}

void console_write_color(const char* str, uint8_t forecolor, uint8_t backcolor) {
    // 保存原来的颜色
    uint8_t old_forecolor = console_forecolor;
    uint8_t old_backcolor = console_backcolor;

    // 设置新颜色
    console_forecolor = forecolor;
    console_backcolor = backcolor;

    console_write(str);

    // 恢复原来的颜色
    console_forecolor = old_forecolor;
    console_backcolor = old_backcolor;
}