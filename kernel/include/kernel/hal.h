#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

void hal_init();

uint8_t hal_inb(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif