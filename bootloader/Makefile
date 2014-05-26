all:
	gcc -g -Os -S -march=i686 -fno-leading-underscore -ffreestanding -Wall -Werror -I. -o bootloader.s bootloader.c
	gcc -g -Os -c -march=i686 -fno-leading-underscore -ffreestanding -Wall -Werror -I. -o bootloader.o bootloader.c
	ld -s -Tlinker.ld -static -nostdlib --nmagic -o bootloader.exe bootloader.o
	objcopy -O binary bootloader.exe bootloader.bin

clean:
	rm -f *.o *.bin *.s *exe

