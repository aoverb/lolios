CC:=i686-elf-gcc
AR:=i686-elf-ar

LIBC_OBJS=\
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
string/strlen.o \

SYSROOT?=

CFLAGS?=-O2 -g
CPPFLAGS?=

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_libc --sysroot=$(SYSROOT) -isystem $(SYSROOT)/usr/include
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

LIBK_OBJS=$(LIBC_OBJS:.o=.libk.o)
BINARIES=libk.a

.PHONY: all clean install-headers

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.libk.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

-include $(LIBK_OBJS:.o=.d)

install-headers:
	cp -R --preserve=timestamps include/. $(SYSROOT)/usr/include/.

clean:
	rm -f $(LIBK_OBJS)
	rm -f $(LIBK_OBJS:.o=.d)