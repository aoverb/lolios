#!/bin/bash
export SYSROOT=`pwd`/SYSROOT
echo $SYSROOT
mkdir -p $SYSROOT/usr/lib
mkdir -p $SYSROOT/usr/include
cd kernel
make install-headers
cd ..
cd libc
make install-headers clean all install-libc
cd ..