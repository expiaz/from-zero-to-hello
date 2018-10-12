ASM = ./asm

C = ./c

OUT = ./out

all: compile kernel concat emul

compile:
	nasm $(ASM)/boot.asm -f bin -o $(OUT)/boot.bin
	
kernel:
	nasm $(ASM)/kernel_entry.asm -f elf -o $(OUT)/kernel_entry.o
	i386-elf-gcc -ffreestanding -c $(C)/kernel.c -o $(OUT)/kernel.o
	i386-elf-ld -o $(OUT)/kernel.bin -Ttext 0x1000 $(OUT)/kernel_entry.o $(OUT)/kernel.o --oformat binary
	rm $(OUT)/kernel.o $(OUT)/kernel_entry.o

concat:
	cat $(OUT)/boot.bin $(OUT)/kernel.bin > $(OUT)/os.img

emul:
	qemu-system-i386 $(OUT)/os.img

link:
	ld -o basic.bin -Ttext 0x0 --oformat binary basic.o