#include "gdt.h"

static gdt_entry_struct gdt_entries[5];

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, 
                  uint8_t dpl, uint8_t executable) 
{
    gdt_entry_struct *entry = &gdt_entries[num];

    entry->base_low    = (base & 0xFFFF);
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high   = (base >> 24) & 0xFF;

    entry->limit_low   = (limit & 0xFFFF);
    entry->limit_high  = (limit >> 16) & 0x0F;

    // 设置 Access 字节
    entry->present         = 1;
    entry->dpl             = dpl;
    entry->descriptor_type = 1; // 代码/数据段
    entry->executable      = executable;
    entry->read_write      = 1;
    entry->conforming_expand = 0;
    entry->accessed        = 0;

    // 设置 Flags
    entry->granularity     = 1; // 4KB 粒度
    entry->default_size    = 1; // 32位模式
    entry->long_mode       = 0;
    entry->available       = 0;
}

void load_gdtr() {
    gdtr_descriptor gdtr_desc;
    gdtr_desc.base = (uint32_t)(&gdt_entries);
    gdtr_desc.limit = sizeof(gdt_entry_struct) * 5 - 1;

    asm volatile("lgdt %0" : : "m"(gdtr_desc));
    
    // 刷新段寄存器
    asm volatile(
        "movw $0x10, %%ax \n\t"
        "movw %%ax, %%ds \n\t"
        "movw %%ax, %%es \n\t"
        "movw %%ax, %%fs \n\t"
        "movw %%ax, %%gs \n\t"
        "movw %%ax, %%ss \n\t"
        "ljmp $0x08, $1f \n\t"  // 远跳转刷新 CS
        "1:"
        : : : "ax"
    );
}

void gdt_init() {
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFF, 0, 1);
    gdt_set_gate(2, 0, 0xFFFFF, 0, 0);
    gdt_set_gate(3, 0, 0xFFFFF, 3, 1);
    gdt_set_gate(4, 0, 0xFFFFF, 3, 0);

    load_gdtr();
}