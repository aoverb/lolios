/* kernel/arch/i386/tty.cpp */
#include <kernel/tty.h>
#include <boot/multiboot.h>
#include <stddef.h>

/* 全局变量：显卡驱动私有，不需要 extern 给别人看 */
static uint32_t* fb_addr;
static uint32_t fb_pitch;
static uint32_t fb_bpp;

static int terminal_x;

static const uint32_t white = 0x00FFFFFF;

/* 字体数据 (私有数据) */
static uint8_t font_H[] = {0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0x00};
static uint8_t font_e[] = {0x00, 0x3C, 0x42, 0x7E, 0x40, 0x3C, 0x00, 0x00};
static uint8_t font_l[] = {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00};
static uint8_t font_o[] = {0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00};
static uint8_t font_W[] = {0xC3, 0xC3, 0xC3, 0xDB, 0xFF, 0xC3, 0x00, 0x00};
static uint8_t font_r[] = {0x00, 0xAE, 0x66, 0x60, 0x60, 0xF0, 0x00, 0x00};
static uint8_t font_d[] = {0x06, 0x06, 0x3E, 0x46, 0x46, 0x3E, 0x00, 0x00};
static uint8_t font_space[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 为了方便外部调用，我们临时把这些暴露出去（或者你可以在这里写个查找函数）
// 在正规 OS 中，这应该是一个完整的 ASCII 表
uint8_t* get_font_bitmap(char c) {
    switch(c) {
        case 'H': return font_H;
        case 'e': return font_e;
        case 'l': return font_l;
        case 'o': return font_o;
        case 'W': return font_W;
        case 'r': return font_r;
        case 'd': return font_d;
        case ' ': return font_space;
        default: return font_space; // 默认
    }
}

void terminal_initialize(multiboot_info_t* mbi) {
    fb_addr = (uint32_t*)(uintptr_t)mbi->framebuffer_addr;
    fb_pitch = mbi->framebuffer_pitch;
    fb_bpp = mbi->framebuffer_bpp;
}

void terminal_putpixel(int x, int y, uint32_t color) {
    // 简单的边界检查是好习惯，防止画出屏幕导致崩溃
    // if (!fb_addr) return; 
    
    uint32_t offset = y * (fb_pitch / 4) + x;
    fb_addr[offset] = color;
}

void terminal_draw_char(int x, int y, const uint8_t* font_char, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((font_char[row] & (0x80 >> col))) {
                terminal_putpixel(x + col, y + row, color);
            }
        }
    }
}

void terminal_fill_rect(int x, int y, int width, int height, uint32_t color) {
    for(int i = y; i < y + height; i++) {
        for(int j = x; j < x + width; j++) {
            terminal_putpixel(j, i, color);
        }
    }
}

void terminal_write(const char* data, size_t size) {
	// todo...
    for (size_t i = 0; i < size; i++) {
        terminal_draw_char(terminal_x, 24, get_font_bitmap(data[i]), white);
        terminal_x += 8;
    }
}
