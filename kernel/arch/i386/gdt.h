#ifndef _ARCH_I386_GDT_H
#define _ARCH_I386_GDT_H
#include <stdint.h>

struct gdt_entry_struct {
    uint16_t limit_low;           // 段限长 (0-15位)
    uint16_t base_low;            // 段基址 (0-15位)
    uint8_t  base_middle;         // 段基址 (16-23位)

    /* Access Byte: 定义段的访问权限 (从低位到高位) */
    uint8_t accessed : 1;         // 是否被访问过 (通常设为 0)
    uint8_t read_write : 1;       // 代码段:可读 / 数据段:可写
    uint8_t conforming_expand : 1;// 代码段:一致性 / 数据段:扩展方向
    uint8_t executable : 1;       // 1=代码段, 0=数据段
    uint8_t descriptor_type : 1;  // 1=代码或数据段, 0=系统段(如TSS)
    uint8_t dpl : 2;              // 特权级 (0=内核, 3=用户)
    uint8_t present : 1;          // 段是否存在 (必须为 1)

    /* Flags + Limit High: 混合字节 (从低位到高位) */
    uint8_t limit_high : 4;       // 段限长 (16-19位)
    uint8_t available : 1;        // 留给软件使用 (通常设为 0)
    uint8_t long_mode : 1;        // 1=64位模式 (32位下设为 0)
    uint8_t default_size : 1;     // 1=32位保护模式, 0=16位
    uint8_t granularity : 1;      // 1=4KB单位, 0=1字节单位

    uint8_t  base_high;           // 段基址 (24-31位)
} __attribute__((packed));

struct gdtr_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#ifdef __cplusplus
extern "C" {
#endif

void gdt_init();

#ifdef __cplusplus
}
#endif

#endif