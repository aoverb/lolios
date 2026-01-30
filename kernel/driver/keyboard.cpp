#include <driver/keyboard.h>
#include <stdio.h>
#include <kernel/hal.h>
#include <kernel/isr.h>

void keyboard_interrupt_handler(registers* /* regs */) {
    uint8_t scancode = hal_inb(0x60);
    printf("%d ", scancode);
    return;
}

void keyboard_init() {
    register_interrupt_handler(33, keyboard_interrupt_handler);
    while (hal_inb(0x64) & 0x01) {
        hal_inb(0x60); // 读走数据，但不做任何处理，直接丢弃
    }
}

char keyboard_getchar() {

}

bool keyboard_haschar() {

}