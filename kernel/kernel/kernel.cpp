/* kernel/kernel/kernel.cpp */
#include <stddef.h>
#include <stdint.h>
// 引入上面定义的两个头文件
#include <boot/multiboot.h>
#include <stdio.h>
#include <kernel/tty.h>

/* 我们还需要手动声明一下 get_font_bitmap，
   或者更好的做法是把它加到 tty.h 里去。
   为了演示方便，先在这里 extern 一下 
*/
extern uint8_t* get_font_bitmap(char c); 

extern "C" void kernel_main(multiboot_info_t* mbi) {
    terminal_initialize(mbi);
    printf("Hello World");
}
