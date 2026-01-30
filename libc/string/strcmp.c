#include <string.h>

int strcmp(const char* str1, const char* str2) {
    while(*str1 == *str2) {
        if (*str1 == '\0' || *str2 == '\0') break;
        ++str1;
        ++str2;
    }
    if (*str1 == *str2) return 0;
    return *str2 - *str1;
}
