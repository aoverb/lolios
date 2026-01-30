#ifndef _DRIVER_KEYBOARD_H
#define _DRIVER_KEYBOARD_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_init();

char keyboard_getchar();

bool keyboard_haschar();

void keyboard_flush();

#ifdef __cplusplus
}
#endif

#endif