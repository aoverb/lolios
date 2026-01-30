#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char*);
int strcmp(const char*, const char*);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif

#endif
