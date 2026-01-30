#ifndef _ARCH_I386_IDT_H
#define _ARCH_I386_IDT_H
#include <stdint.h>

#define SET_ISR(n) \
    idt_set_gate(n, (uint32_t)isr##n, 0x08, 0)

#define DEF_ISR(n) \
    extern "C" void isr##n()

DEF_ISR(0);
DEF_ISR(1);
DEF_ISR(2);
DEF_ISR(3);
DEF_ISR(4);
DEF_ISR(5);
DEF_ISR(6);
DEF_ISR(7);
DEF_ISR(8);
DEF_ISR(9);
DEF_ISR(10);
DEF_ISR(11);
DEF_ISR(12);
DEF_ISR(13);
DEF_ISR(14);
DEF_ISR(15);
DEF_ISR(16);
DEF_ISR(17);
DEF_ISR(18);
DEF_ISR(19);
DEF_ISR(20);
DEF_ISR(21);
DEF_ISR(22);
DEF_ISR(23);
DEF_ISR(24);
DEF_ISR(25);
DEF_ISR(26);
DEF_ISR(27);
DEF_ISR(28);
DEF_ISR(29);
DEF_ISR(30);
DEF_ISR(31);

struct idt_entry_struct {
    uint16_t offset_low;          // 中断处理函数地址的 (0-15位)
    uint16_t selector;            // 目标代码段选择子 (在GDT中的索引，也就是0x08)
    uint8_t  zero;                // 必须置 0 的保留位

    /* Type and Attributes: 定义中断门的属性 (从低位到高位) */
    uint8_t gate_type : 4;        // 门类型: 0xE (32-bit Interrupt Gate), 0xF (32-bit Trap Gate)
    uint8_t storage_segment : 1;  // 对于中断门，该位必须为 0 (系统段)
    uint8_t dpl : 2;              // 特权级 (0=内核, 3=用户)
    uint8_t present : 1;          // 段是否存在 (必须为 1 才能触发中断)

    uint16_t offset_high;         // 中断处理函数地址的 (16-31位)
} __attribute__((packed));

struct idtr_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha 压入
    uint32_t int_no, err_code;                       // 我们手动压入
    uint32_t eip, cs, eflags, useresp, ss;           // CPU 自动压入
};

#ifdef __cplusplus
extern "C" {
#endif

void idt_init();
void inner_interrupt_handler(registers* regs);

#ifdef __cplusplus
}
#endif

#endif