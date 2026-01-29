#include "gdt.h"

static gdt_entry_struct gdt_entries[5];

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, 
                  uint8_t dpl, uint8_t executable) 
{
    gdt_entry_struct *entry = &gdt_entries[num];

    entry->base_low    = (base & 0xFFFF);
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high   = (base >> 24) & 0xFF;

    uint32_t real_limit = (limit >> 12); 
    entry->limit_low   = (real_limit & 0xFFFF);
    entry->limit_high  = (real_limit >> 16) & 0x0F;

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

    asm volatile (
        "lgdt %0 \n\t"
        "ljmp $0x08, $1f \n\t"
        "1: \n\t"
        "mov $0x10, %%ax \n\t"
        "mov %%ax, %%ds \n\t"
        "mov %%ax, %%es \n\t"
        "mov %%ax, %%fs \n\t"
        "mov %%ax, %%gs \n\t"
        "mov %%ax, %%ss"
        :
        : "m"(gdtr_desc)
        : "ax", "memory"
    );
}

void gdt_init() {
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0, 1);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0, 0);
    gdt_set_gate(3, 0, 0xFFFFFFFF, 3, 1);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 3, 0);

    load_gdtr();
}