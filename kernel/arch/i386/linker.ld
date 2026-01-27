/* linker.ld */
ENTRY(_start)

SECTIONS
{
	/* 内核从 1MB 处开始 */
	. = 1M;

	/* Multiboot 头必须在最前面 */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* 只读数据 */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* 已初始化数据 */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* 未初始化数据和栈 (BSS) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
