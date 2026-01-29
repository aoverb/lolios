.section .text
.extern inner_interrupt_handler
.global isr0
.global isr6
.global common_interrupt_handler

isr0: #DE Divide-by-Zero
    pushl $0
    pushl $0
    jmp common_interrupt_handler

isr6: #Invalid Opcode
    pushl $0
    pushl $6
    jmp common_interrupt_handler
    
common_interrupt_handler:
    pusha
    pushl %esp
    call inner_interrupt_handler
    addl $4, %esp
    popa
    addl $8, %esp
    iret
