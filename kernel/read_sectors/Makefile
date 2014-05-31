all:
	as -o boot.o boot.s
	ld -e my_main -o boot.out boot.o -Ttext 0x7C00
	objcopy -O binary -j .text boot.out boot.bin
	mkdir -p iso_content
	dd if=boot.bin of=./iso_content/boot.img ibs=1474560 conv=sync
	mkisofs -J -R -o boot.iso -b boot.img iso_content

clean:
	rm -rf *.o *.out *.bin iso_content *.iso