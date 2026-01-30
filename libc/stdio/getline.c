#include <stdint.h>
#include <stdio.h>
#include <driver/keyboard.h>

void getline(char* buf, uint32_t size) {
    keyboard_flush();
    uint32_t i = 0;

    while (i < size - 1) {
        while (!keyboard_haschar()) {
            asm volatile("pause"); 
        }

        char c = keyboard_getchar();

        if (c == '\b') {
            if (i == 0) continue;
            --i;
            printf("\b");
            continue;
        }

        if (c == '\n') {
            buf[i] = '\0';
            printf("\n");
            return;
        }

        if (c >= 32 && c <= 126) {
            buf[i++] = c;
            printf("%c", c);
        }
    }

    buf[i] = '\0';
}