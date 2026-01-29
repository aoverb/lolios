#include "idt.h"
#include <stdio.h>
#include <string.h>

idt_entry_struct idt_entries[256];

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t dpl) {
    idt_entries[num].offset_low  = base & 0xFFFF;
    idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector    = sel;
    idt_entries[num].zero        = 0;
    idt_entries[num].gate_type       = 0xE;
    idt_entries[num].storage_segment = 0;
    idt_entries[num].dpl            = dpl;
    idt_entries[num].present        = 1;
}

void inner_interrupt_handler(registers* regs) {
    set_color(0x00FF0000);
    printf("int: %d\n", regs->int_no);
    printf("An critical error has occurred: %d\n", regs->err_code);
    for (;;) {
        asm volatile("hlt");
    }
    return;
}

void idt_init() {
    memset(idt_entries, 0, sizeof(idt_entry_struct) * 256);

    idtr_descriptor idtr_sel;
    idtr_sel.base = (uint32_t)(&idt_entries);
    idtr_sel.limit = sizeof(idt_entry_struct) * 256 - 1;

    idt_set_gate(0, (uint32_t)(&isr0), 0x08, 0);
    idt_set_gate(6, (uint32_t)(&isr6), 0x08, 0);

    asm volatile(
        "lidt %0"
        :
        : "m"(idtr_sel)
        : "memory"
    );
    return;
}