#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

void hal_init();

uint8_t hal_inb(uint16_t port);
void hal_outb(uint16_t port, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif