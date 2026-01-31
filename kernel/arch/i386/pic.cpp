#include "pic.h"

void pic_init() {
    outb(0x20, 0x11); // 初始化主片
    outb(0xA0, 0x11); // 初始化从片
    outb(0x21, 0x20); // 主片偏移设为 0x20
    outb(0xA1, 0x28); // 从片偏移设为 0x28
    outb(0x21, 0x04); // 告知主片 IRQ2 连接从片
    outb(0xA1, 0x02); // 告知从片连接主片 IRQ2
    outb(0x21, 0x01); // 8086 模式
    outb(0xA1, 0x01);

    // 先屏蔽所有IRQ，由驱动打开
    outb(0x21, 0xFF); 
    outb(0xA1, 0xFF);
}

void pic_enable_irq(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

// 屏蔽特定 IRQ 线
void pic_disable_irq(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}