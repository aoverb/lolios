#!/bin/bash
i686-elf-as boot.s -o boot.o
i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
# 复制内核
cp myos.bin isodir/boot/myos.bin

# 创建 GRUB 配置文件
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "My First OS" {
	multiboot /boot/myos.bin
}
EOF

# 生成 ISO 文件
grub-mkrescue -o myos.iso isodir
qemu-system-i386 -cdrom myos.iso
