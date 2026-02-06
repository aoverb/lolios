#include <kernel/panic.h>
#include <kernel/tty.h>
#include <string.h>

void panic(const char* str) {
    terminal_write(str, strlen(str));
    while(1) __asm__ volatile("hlt");
}