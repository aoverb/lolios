#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <boot/multiboot.h>

/* 如果是 C++ 环境，告诉编译器这部分按 C 的链接规则处理 */
#ifdef __cplusplus
extern "C" {
#endif

void terminal_initialize(multiboot_info_t* mbi);
void terminal_putpixel(int x, int y, uint32_t color);
void terminal_draw_char(int x, int y, const uint8_t* font_char, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif
