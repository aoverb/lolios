/* kernel/kernel/kernel.cpp */
#include <stddef.h>
#include <stdint.h>
// 引入上面定义的两个头文件
#include <boot/multiboot.h>
#include <stdio.h>
#include <kernel/tty.h>

extern "C" void kernel_main(multiboot_info_t* mbi) {
    terminal_initialize(mbi);
    printf("Hello World and welcome to the kernel world! \nA quick brown fox jumps over the lazy dog.");
    printf("\nA quick brown fox jumps over the lazy dog.");
    printf("\nA quick brown fox jumps over the lazy dog.");
    printf("\nA quick brown fox jumps over the lazy dog.");
}
