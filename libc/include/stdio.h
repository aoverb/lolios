#ifndef _STDIO_H
#define _STDIO_H 1

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...) __attribute__((format(printf, 1, 2)));
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
