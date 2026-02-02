#ifndef _DRIVER_PIT_H
#define _DRIVER_PIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void pit_init();
void pit_sleep(uint32_t);
uint32_t pit_get_ticks();

#ifdef __cplusplus
}
#endif

#endif