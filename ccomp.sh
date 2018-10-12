NAME=$1

gcc -ffreestanding -c c/$NAME.c -o $NAME.o
ld -o $NAME.bin -Ttext 0x0 --oformat binary $NAME.o
ndisasm -b 32 $NAME.bin > $NAME.dis

rm $NAME.o $NAME.bin