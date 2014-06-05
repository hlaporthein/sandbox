all:
	gcc -S -masm=intel -fno-asynchronous-unwind-tables -fno-leading-underscore -ffreestanding -Wall -Werror -o boot.s boot.c
	as -o boot.o boot.s
	gcc -S -masm=intel -fno-asynchronous-unwind-tables -fno-leading-underscore -ffreestanding -Wall -Werror -o boot2.s boot2.c
	as -o boot2.o boot2.s
	objcopy --rename-section .text=.boot --rename-section .rdata=.bootd boot.o
	ld -o boot.out -T linker.ld boot.o boot2.o
	objcopy -O binary boot.out boot.bin
	mkdir -p iso_content
	dd if=boot.bin of=./iso_content/boot.img ibs=1474560 conv=sync
	mkisofs -J -R -o boot.iso -b boot.img iso_content

clean:
	rm -rf *.s *.o *.out *.bin iso_content *.iso
