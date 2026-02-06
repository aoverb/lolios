#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H
#include <stdint.h>
struct multiboot_mmap_entry
{
  uint32_t size;
  uint64_t addr;
  uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
} __attribute__((packed));
#define MAX_ORDER 12

typedef struct multiboot_mmap_entry multiboot_memory_map_t;

typedef struct pm_entry {
    uint32_t begin, end;
} pm_entry;

typedef struct pm_list {
    pm_entry entries[128];
    uint32_t count;
} pm_list;

typedef struct page_frame page_frame;

struct page_frame {
    page_frame *prev, *next;
    uint8_t order;
    uint8_t allocated;
};

page_frame* all_pages;

page_frame* free_area[MAX_ORDER];

uint32_t page_limit;

void pmm_init(pm_list* pms);

void* pmm_alloc(uint32_t size);

void pmm_free(void* addr);

void pmm_probe();

#endif