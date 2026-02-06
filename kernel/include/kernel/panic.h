#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void panic(const char* str);

#ifdef __cplusplus
}
#endif

#endif