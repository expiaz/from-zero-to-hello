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

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o boot/idt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g
QFLAGS = -fda #use -hda for hard drive image is size >= 3kb

KERNEL = kernel
BOOT = boot
DRIVER = drivers

all: os.img

run: os.img
	qemu-system-i386 ${QFLAGS} os.img

debug: os.img kernel.elf
	qemu-system-i386 -s -S os.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

os.img: ${BOOT}/boot.bin kernel.bin
	cat $^ > $@

kernel.bin: ${BOOT}/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: ${BOOT}/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I 'boot/' -o $@

clean:
	rm -rf *.bin *.dis *.o os.img *.elf
	rm -rf ${KERNEL}/*.o ${BOOT}/*.bin ${DRIVER}/*.o ${BOOT}/*.o