#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

void set_color(uint32_t color) {
#if defined(__is_libk)
	terminal_setcolor(color);
#else
	// TODO: Implement stdio and the write system call.
#endif
}