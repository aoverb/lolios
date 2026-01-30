#ifndef _DRIVER_KEYBOARD_H
#define _DRIVER_KEYBOARD_H


#ifdef __cplusplus
extern "C" {
#endif

void keyboard_init();

char keyboard_getchar();

bool keyboard_haschar();

#ifdef __cplusplus
}
#endif

#endif