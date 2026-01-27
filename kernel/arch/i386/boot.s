/* boot.s */
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set VIDEOMODE, 1<<2        /* 新增：请求视频模式标志位 */
.set FLAGS,    ALIGN | MEMINFO | VIDEOMODE
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
/* Multiboot 头部特定地址字段 (如果不是 ELF 格式才需要，这里填 0) */
.long 0, 0, 0, 0, 0

/* 图形模式请求设置 */
.long 0                     /* 0 = 线性图形模式 */
.long 1024                  /* 宽度 */
.long 768                   /* 高度 */
.long 32                    /* 像素深度 (32-bit: R,G,B,Alpha) */

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp

    /* 关键：将 Multiboot 信息结构的地址 (存在 ebx 中) 压入栈，传给 C++ */
    push %ebx
    
    call kernel_main

    cli
hltLoop:
    hlt
    jmp hltLoop
