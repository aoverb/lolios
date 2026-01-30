#ifndef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include <stdint.h>

struct registers {
    uint32_t gs, fs, es, ds;      // 对应 pop %gs ... pop %ds
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha 压入
    uint32_t int_no, err_code;                       // 我们手动压入
    uint32_t eip, cs, eflags, useresp, ss;           // CPU 自动压入
};

typedef void (*interrupt_handler_t)(registers*);

#ifdef __cplusplus
extern "C" {
#endif

extern interrupt_handler_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, interrupt_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif
