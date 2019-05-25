#! /bin/sh

# [https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler]
# [https://wiki.osdev.org/GCC_Cross-Compiler]

# Prerequisites

Brew install gmp mpfr libmpc gcc

export CC=/usr/local/bin/gcc-9
export LD=/usr/local/bin/gcc-9
export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

# Binutils
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.gz
tar xf binutils-2.32.tar.gz
mkdir binutils-build && cd binutils-build
../binutils-2.24/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX —-with-sysroot
make
make install

# GCC
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-9.1.0/gcc-9.1.0.tar.gz
tar xf gcc-9.1.0.tar.gz
mkdir gcc-build && cd gcc-build
../gcc-9.1.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --disable-libssp --enable-languages=c,c++ --without-headers --enable-interwork --enable-multilib --with-gmp=/usr/local/Cellar/gmp/6.1.2_2/ --with-mpc=/usr/local/Cellar/libmpc/1.1.0/ --with-mpfr=/usr/local/Cellar/mpfr/4.0.2/
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# GDB
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gdb/gdb-8.3.tar.gz
tar xf gdb-8.3.tar.gz
mkdir gdb-build && cd gdb-build
../gdb-8.3/configure --target=$TARGET --prefix=$PREFIX --program-prefix=i386-elf-
make
make install
