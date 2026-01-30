#include <driver/keyboard.h>
#include <stdio.h>
#include <kernel/hal.h>
#include <kernel/isr.h>

#define SCAN_CODE_MAX 0x84

/* 定义一个足够大的数组覆盖到 0x66 (Backspace) */
const char scancode_to_ascii_table[128][2] = {
    /* 0x00 - 0x07 */ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
    /* 0x08 - 0x0F */ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'`','~'}, {0,0},
    /* 0x10 - 0x17 */ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'q','Q'}, {'1','!'}, {0,0},
    /* 0x18 - 0x1F */ {0,0}, {0,0}, {'z','Z'}, {'s','S'}, {'a','A'}, {'w','W'}, {'2','@'}, {0,0},
    /* 0x20 - 0x27 */ {0,0}, {'c','C'}, {'x','X'}, {'d','D'}, {'e','E'}, {'4','$'}, {'3','#'}, {0,0},
    /* 0x28 - 0x2F */ {0,0}, {' ',' '}, {'v','V'}, {'f','F'}, {'t','T'}, {'r','R'}, {'5','%'}, {0,0},
    /* 0x30 - 0x37 */ {0,0}, {'n','N'}, {'b','B'}, {'h','H'}, {'g','G'}, {'y','Y'}, {'6','^'}, {0,0},
    /* 0x38 - 0x3F */ {0,0}, {0,0}, {'m','M'}, {'j','J'}, {'u','U'}, {'7','&'}, {'8','*'}, {0,0},
    /* 0x40 - 0x47 */ {0,0}, {',','<'}, {'k','K'}, {'i','I'}, {'o','O'}, {'0',')'}, {'9','('}, {0,0},
    /* 0x48 - 0x4F */ {0,0}, {'.','>'}, {'/','?'}, {'l','L'}, {';',':'}, {'p','P'}, {'-','_'}, {0,0},
    /* 0x50 - 0x57 */ {0,0}, {0,0}, {'\'','\"'}, {0,0}, {'[','{'}, {'=','+'}, {0,0}, {0,0},
    /* 0x58 - 0x5F */ {0,0}, {0,0}, {'\n','\n'}, {']','}'}, {0,0}, {'\\','|'}, {0,0}, {0,0},
    /* 0x60 - 0x67 */ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'\b','\b'}, {0,0}
};

bool releasing = false;
bool is_shift_pressed = false;

struct round_buffer {
    volatile char buffer[256];
    volatile uint8_t head = 0;
    volatile uint8_t tail = 0;
} kbd_buffer;

void rb_flush() {
    kbd_buffer.head = kbd_buffer.tail = 0;
}

char rb_read(round_buffer* buf) {
    asm volatile ("cli");
    if (buf->head == buf->tail) {
        asm volatile ("sti");
        return -1;
    }
    char re = buf->buffer[buf->head++];
    asm volatile ("sti");
    return re;
    
}

void rb_write(round_buffer* buf, char data) {
    asm volatile ("cli");
    buf->buffer[(buf->tail)++] = data;
    if (buf->head == buf->tail) {
        ++(buf->head);
    }
    asm volatile ("sti");
}

void keyboard_interrupt_handler(registers* /* regs */) {
    uint8_t scancode = hal_inb(0x60);
    if (scancode == 0xF0) {
        releasing = true;
        return;
    }
    if (scancode == 0x12) {
        is_shift_pressed = !releasing;
    }
    releasing = false;
    rb_write(&kbd_buffer, scancode_to_ascii_table[scancode][is_shift_pressed]);
    return;
}

void keyboard_init() {
    rb_flush();
    register_interrupt_handler(33, keyboard_interrupt_handler);
    while (hal_inb(0x64) & 0x01) {
        hal_inb(0x60); // 读走数据，但不做任何处理，直接丢弃
    }
}

char keyboard_getchar() {
    return rb_read(&kbd_buffer);
}

bool keyboard_haschar() {
    return kbd_buffer.head != kbd_buffer.tail;
}