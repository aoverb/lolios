/* kernel/arch/i386/tty.cpp */
#include <kernel/tty.h>
#include <kernel/font.h>
#include <string.h>
#include <boot/multiboot.h>
#include <stddef.h>
#include <stdint.h> // for uint8_t etc.

extern uint32_t page_directory;

static uint32_t* fb_addr;
static uint32_t fb_pitch;
static uint32_t fb_bpp;
static uint32_t fb_width;   // 屏幕宽度（像素）
static uint32_t fb_height;  // 屏幕高度（像素）
static uint32_t terminal_cols;  // 字符列数
static uint32_t terminal_rows;  // 字符行数

static uint32_t terminal_row = 0;
static uint32_t terminal_col = 0;

static uint32_t terminal_color = 0x00FFFFFF;

/* framebuffer helpers (unchanged) */

// 丑陋的stub，以后有了kmalloc和正确的分页机制要去掉
void map_lfb_hardcore(uint32_t phys_addr, uint32_t size) {
    // 强制开启 PSE
    asm volatile(
        "mov %%cr4, %%eax\n"
        "or $0x10, %%eax\n"
        "mov %%eax, %%cr4"
        : : : "eax"
    );

    uint32_t pde_index = 832; // 0xD0000000
    uint32_t num_pages = (size + 0x3FFFFF) / 0x400000;
    
    // 我们直接用汇编往内存里写，不经过 C 数组索引，防止指针寻址出错
    // 这里的 0xC0106000 必须是你 page_directory 的准确虚拟地址
    uint32_t* pd_ptr = &page_directory; 

    for (uint32_t i = 0; i < num_pages; i++) {
        uint32_t entry_val = (phys_addr + (i * 0x400000)) | 0x83;
        pd_ptr[pde_index + i] = entry_val;
    }

    // 刷新 TLB
    asm volatile("mov %%cr3, %%eax\n mov %%eax, %%cr3" : : : "eax");
}

void terminal_initialize(multiboot_info_t* mbi) {
    fb_addr = (uint32_t*)(uintptr_t)mbi->framebuffer_addr;
    fb_pitch = mbi->framebuffer_pitch;
    fb_bpp = mbi->framebuffer_bpp;

    /* 从 multiboot 获取分辨率 */
    fb_width = mbi->framebuffer_width;
    fb_height = mbi->framebuffer_height;
    
    /* 计算字符行列数 */
    terminal_cols = fb_width / FONT_WIDTH;
    terminal_rows = fb_height / FONT_HEIGHT;


    uint32_t lfb_physical_addr = mbi->framebuffer_addr;
    uint32_t lfb_size = mbi->framebuffer_width * mbi->framebuffer_height * (mbi->framebuffer_bpp / 8);
    map_lfb_hardcore(lfb_physical_addr, lfb_size);
    fb_addr = (uint32_t*)(uintptr_t)0xD0000000; // ...
}

void terminal_setcolor(uint32_t color) {
    terminal_color = color;
}

void terminal_putpixel(int x, int y, uint32_t color) {
    uint32_t offset = y * (fb_pitch / 4) + x;
    fb_addr[offset] = color;
}


void terminal_draw_char(int x, int y, const uint8_t* font_char, uint32_t color) {
    // 控制台绘制字体的逻辑不应该依赖于字体的具体实现！需要重构。但是现在只是用来简单输出字符，刚刚好够用。
    for (int row = 0; row < FONT_HEIGHT; row++) {
        for (int col = 0; col < FONT_WIDTH; col++) {
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

void terminal_scroll() {
    size_t line_size = fb_pitch;
    
    for (uint32_t row = 1; row < terminal_rows; row++) {
        uint8_t* src = (uint8_t*)fb_addr + row * FONT_HEIGHT * line_size;
        uint8_t* dst = (uint8_t*)fb_addr + (row - 1) * FONT_HEIGHT * line_size;
        
        for (int i = 0; i < FONT_HEIGHT; i++) {
            memcpy(dst + i * line_size, src + i * line_size, FONT_HEIGHT * line_size);
        }
    }
    
    // 清空最后一行
    terminal_fill_rect(0, (terminal_rows - 1) * FONT_HEIGHT, 
                      fb_width, FONT_HEIGHT, 0x00000000);
    
    terminal_row = terminal_rows - 1;
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\b') {
            if (terminal_col == 0 && terminal_row == 0) {
                return;
            }
            if (terminal_col == 0) {
                --terminal_row;
                terminal_col = terminal_cols;
            }
            --terminal_col;
            terminal_fill_rect(terminal_col * FONT_WIDTH, 
                              terminal_row * FONT_HEIGHT, 
                              FONT_WIDTH, 
                              FONT_HEIGHT, 
                              0x00000000);
            continue;
        }
        if (data[i] == '\n') {
            ++terminal_row;
            terminal_col = 0;
            continue;
        }
        if (terminal_col >= terminal_cols) {
            ++terminal_row;
            terminal_col = 0;
        }
        if (terminal_row >= terminal_rows) {
            terminal_scroll();
        }
        unsigned char c = (unsigned char)data[i];
        const uint8_t* glyph = font_8x16[c];
        terminal_draw_char(terminal_col++ * FONT_WIDTH, terminal_row * FONT_HEIGHT, glyph, terminal_color);
    }
}
