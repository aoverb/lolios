#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include <kernel/hal.h>

void hal_init() {
    asm volatile ("cli");
    gdt_init();
    pic_init();
    idt_init();
    io_init();
}

uint8_t hal_inb(uint16_t port) {
    inb(port);
}

void hal_outb(uint16_t port, uint8_t val) {
    outb(port, val);
}
