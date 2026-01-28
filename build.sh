#!/bin/bash
. build_libc.sh
cd kernel
make
cd ..
# 复制内核
cp kernel/myos.kernel isodir/boot/myos.bin

# 创建 GRUB 配置文件
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "My First OS" {
	multiboot /boot/myos.bin
}
EOF

# 生成 ISO 文件
grub-mkrescue -o myos.iso isodir
qemu-system-i386 -cdrom myos.iso
