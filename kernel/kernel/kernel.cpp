/* kernel/kernel/kernel.cpp */
#include <stddef.h>
#include <stdint.h>
// 引入上面定义的两个头文件
#include <boot/multiboot.h>
#include <kernel/tty.h>

/* 我们还需要手动声明一下 get_font_bitmap，
   或者更好的做法是把它加到 tty.h 里去。
   为了演示方便，先在这里 extern 一下 
*/
extern uint8_t* get_font_bitmap(char c); 

extern "C" void kernel_main(multiboot_info_t* mbi) {
    // 1. 初始化硬件
    terminal_initialize(mbi);

    // 2. 定义颜色
    uint32_t white = 0x00FFFFFF;
    uint32_t green = 0x0000FF00;

    // 3. 业务逻辑：打印字符
    // 现在的代码读起来更像自然语言了
    terminal_draw_char(100, 100, get_font_bitmap('H'), white);
    terminal_draw_char(108, 100, get_font_bitmap('e'), white);
    terminal_draw_char(116, 100, get_font_bitmap('l'), white);
    terminal_draw_char(124, 100, get_font_bitmap('l'), white);
    terminal_draw_char(132, 100, get_font_bitmap('o'), white);
    
    terminal_draw_char(148, 100, get_font_bitmap('W'), white);
    terminal_draw_char(156, 100, get_font_bitmap('o'), white);
    terminal_draw_char(164, 100, get_font_bitmap('r'), white);
    terminal_draw_char(172, 100, get_font_bitmap('l'), white);
    terminal_draw_char(180, 100, get_font_bitmap('d'), white);
}
