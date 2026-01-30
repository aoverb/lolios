#ifndef _ARCH_I386_IO_H
#define _ARCH_I386_IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void io_init();

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

#ifdef __cplusplus
}
#endif

#endif