#include "gdt.h"
#include <kernel/hal.h>

void hal_init() {
    gdt_init();
}
