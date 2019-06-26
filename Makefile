# $@ = target file
# $< = first dependency
# $^ = all dependencies
#
# dependency : target or phoney
# target: target (file) or phoney (name)
# command: shell command
# ----------------------------------
# target: dependency1 dependency2
# 	command
# $@ => target
# $< => dependency1
# $^ => dependency1 dependency2

# to see the executed rules : make -pn

# qemu mouse => crtl+alt+g

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

KERNEL = kernel
BOOT = boot
OUT = out
DRIVER = drivers

KERNEL_ADDR		= 0x8E00
STAGE_2_ADDR	= 0x500

OBJ = ${C_SOURCES:.c=.o kernel/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gd

CFLAGS = -g
QFLAGS = -fda #use -hda for hard drive image of size >= 3kb

all: os.img

run: os.img
	qemu-system-i386 ${QFLAGS} ${OUT}/os.img

debug: os.img kernel.elf
	qemu-system-i386 -s -S ${QFLAGS} ${OUT}/os.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file ${OUT}/kernel.elf"

os.img: ${BOOT}/boot.bin kernel.bin
	cat $^ > ${OUT}/$@

kernel.bin: ${BOOT}/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: ${BOOT}/kernel_entry.o ${OBJ}
	${LD} -o ${OUT}/$@ -Ttext 0x1000 $^

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I 'boot/' -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf
	rm -rf ${KERNEL}/*.o ${BOOT}/*.bin ${DRIVER}/*.o ${BOOT}/*.o