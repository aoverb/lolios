/* kernel.cpp */
#include <stdint.h>
#include <stddef.h>

/* 1. 定义 Multiboot 信息结构体 (简化版)
   我们需要读取其中的 framebuffer_addr 等字段
*/
struct multiboot_info_t {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    
    /* Framebuffer 信息 */
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
};

/* 2. 定义一个极其简单的 8x8 像素字体 
   为了演示，这里只定义 'H', 'e', 'l', 'o', 'W', 'r', 'd' 的位图。
   实际 OS 会引入 font8x8_basic.h 包含所有 ASCII 字符。
   1 表示画点，0 表示背景。
*/
uint8_t font_H[] = {0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0x00};
uint8_t font_e[] = {0x00, 0x3C, 0x42, 0x7E, 0x40, 0x3C, 0x00, 0x00};
uint8_t font_l[] = {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00};
uint8_t font_o[] = {0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00};
uint8_t font_W[] = {0xC3, 0xC3, 0xC3, 0xDB, 0xFF, 0xC3, 0x00, 0x00}; // Simplified
uint8_t font_r[] = {0x00, 0xAE, 0x66, 0x60, 0x60, 0xF0, 0x00, 0x00}; // Simplified
uint8_t font_d[] = {0x06, 0x06, 0x3E, 0x46, 0x46, 0x3E, 0x00, 0x00};

// 全局变量用于存储屏幕信息
uint32_t* fb_addr;
uint32_t fb_pitch;
uint32_t fb_bpp;

/* 3. 画点函数 (Plot Pixel) 
   核心公式：地址 = 基址 + (y * 每行字节数) + (x * 每个像素字节数)
*/
void put_pixel(int x, int y, uint32_t color) {
    // 假设是 32 位颜色 (4 bytes)
    uint32_t offset = y * (fb_pitch / 4) + x;
    fb_addr[offset] = color;
}

/* 4. 绘制字符函数 */
void draw_char(int x, int y, uint8_t* font_char, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // 检查位图中对应的位是否为 1
            // 0x80 是 10000000，用来测试最高位
            if ((font_char[row] & (0x80 >> col))) {
                put_pixel(x + col, y + row, color);
            }
        }
    }
}

/* 内核入口 */
extern "C" void kernel_main(multiboot_info_t* mbi) {
    // 初始化全局变量 (注意：实际代码应检查 flags 确保 framebuffer 有效)
    fb_addr = (uint32_t*) (uint32_t) mbi->framebuffer_addr;
    fb_pitch = mbi->framebuffer_pitch;
    fb_bpp = mbi->framebuffer_bpp;

    // 定义颜色 (0x00RRGGBB)
    uint32_t white = 0x00FFFFFF;
    uint32_t green = 0x0000FF00;

    // 清屏 (简单的涂黑) - 这是一个很慢的操作，以后要优化
    // for(int i=0; i<1024*768; i++) fb_addr[i] = 0x00000000;

    // 在 (100, 100) 处打印 Hello World
    draw_char(100, 100, font_H, white);
    draw_char(108, 100, font_e, white);
    draw_char(116, 100, font_l, white);
    draw_char(124, 100, font_l, white);
    draw_char(132, 100, font_o, white);
    draw_char(148, 100, font_W, white);
    draw_char(156, 100, font_o, white);
    draw_char(164, 100, font_r, white);
    draw_char(172, 100, font_l, white);
    draw_char(180, 100, font_d, white);
    // 绘制一个简单的矩形图形，证明是图形模式
    for(int y=120; y<150; y++) {
        for(int x=100; x<200; x++) {
            put_pixel(x, y, green);
        }
    }
}
