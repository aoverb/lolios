#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict str, ...) {
    // 忽略格式，先写一个直接输出的简单逻辑
    print(str, strlen(str));
    return 0;
}