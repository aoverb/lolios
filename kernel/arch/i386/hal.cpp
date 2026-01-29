#include "gdt.h"
#include "idt.h"
#include <kernel/hal.h>

void hal_init() {
    gdt_init();
    idt_init();
}
