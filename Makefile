# $@ = target file
# $< = first dependency
# $^ = all dependencies
#
# dependency : file or rule name
# rulename: file or ascii name
# command: shell command
# ----------------------------------
# rulename: dependency1 dependency2
# 	command
# $@ => rulename
# $< => dependency1
# $^ => dependency1 dependency2

all: emul

boot.bin: boot.asm
	nasm $< -f bin -o $@

kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

kernel.o: kernel.c
	i386-elf-gcc -ffreestanding -c $< -o $@

kernel.bin: kernel_entry.o kernel.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

os.img: boot.bin kernel.bin
	cat $^ > $@

os.dis: os.img
	ndisasm -b 32 $< > $@
	$(fclean)

clean:
	rm *.o *.bin

fclean: clean
	rm os.img os.dis

emul: os.img clean
	qemu-system-i386 os.img