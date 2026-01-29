/* kernel/arch/i386/tty.cpp */
#include <kernel/tty.h>
#include <kernel/font.h>
#include <boot/multiboot.h>
#include <stddef.h>
#include <stdint.h> // for uint8_t etc.

static uint32_t* fb_addr;
static uint32_t fb_pitch;
static uint32_t fb_bpp;

static int terminal_row = 0;
static int terminal_col = 0;

static const uint32_t white = 0x00FFFFFF;

/* framebuffer helpers (unchanged) */
void terminal_initialize(multiboot_info_t* mbi) {
    fb_addr = (uint32_t*)(uintptr_t)mbi->framebuffer_addr;
    fb_pitch = mbi->framebuffer_pitch;
    fb_bpp = mbi->framebuffer_bpp;
}

void terminal_putpixel(int x, int y, uint32_t color) {
    uint32_t offset = y * (fb_pitch / 4) + x;
    fb_addr[offset] = color;
}

void terminal_draw_char(int x, int y, const uint8_t* font_char, uint32_t color) {
    // 控制台绘制字体的逻辑不应该依赖于字体的具体实现！需要重构。但是现在只是用来简单输出字符，刚刚好够用。
    for (int row = 0; row < 16; row++) {
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
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            ++terminal_col;
            terminal_row = 0;
            continue;
        }
        unsigned char c = (unsigned char)data[i];
        const uint8_t* glyph = font_8x16[c];
        terminal_draw_char(terminal_row++ * 8, terminal_col * 16, glyph, white);
    }
}
