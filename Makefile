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


C_SOURCES		= $(wildcard kernel/*.c drivers/*.c)
ASM_SOURCES		= $(wildcard kernel/*.asm drivers/*.asm)

KERNEL_ADDR		= 0x8E00
STAGE_2_ADDR	= 0x0500
STAGE_1_ADDR	= 0x7C00

OUT_DIR			= out
BOOT_DIR		= boot

OBJ 			= $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)

CC				= /usr/local/i386elfgcc/bin/i386-elf-gcc
LD				= /usr/local/i386elfgcc/bin/i386-elf-ld
GDB				= /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g -I include
QFLAGS = -fda #use -hda for hard drive image of size >= 3kb

all: os.img

run: os.img
	qemu-system-i386 -drive format=raw,file=$(OUT_DIR)/os.img

debug: os.img kernel.elf
	qemu-system-i386 -s -S $(QFLAGS) $(OUT_DIR)/os.img &
	$(GDB)  -ex "target remote localhost:1234" \
	        -ex "set architecture i8086" \
			-ex "set disassembly-flavor intel" \
			-ex "symbol-file $(OUT_DIR)/kernel.elf"

# concatenation of bootloader and kernel
os.img: $(BOOT_DIR)/stage1.bin $(BOOT_DIR)/stage2.bin kernel.bin
	cat $(addprefix $(OUT_DIR)/,$(notdir $^)) > $(OUT_DIR)/$@

# compiled version of kernel
kernel.bin: $(BOOT_DIR)/kernel_entry.o $(OBJ)
	$(LD) -o $(OUT_DIR)/$@ -Ttext $(KERNEL_ADDR) \
	$(addprefix $(OUT_DIR)/,$(notdir $^)) --oformat binary

# symbol version of kernel for debugging
kernel.elf: $(BOOT_DIR)/kernel_entry.o $(OBJ)
	$(LD) -o $(OUT_DIR)/$@ -Ttext $(KERNEL_ADDR) \
	$(addprefix $(OUT_DIR)/,$(notdir $^))

# c files compilation
%.o: %.c
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $(OUT_DIR)/$(notdir $@)

# assembly files for kernel linking
%.o: %.asm
	nasm $< -f elf -o $(OUT_DIR)/$(notdir $@)

# assembly files for bootloader
%.bin: %.asm
	nasm $< -f bin -I boot -o $(OUT_DIR)/$(notdir $@)

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.bin' -delete
	rm out/*