all:
	gcc -c -I/mingw/include/ddk -o driver.o driver.c
	# need for a RELOCATABLE DLL (ie a dll that can be loaded at any virtual address)
	# so the command are a little bit more complex than 'gcc -shared ... driver.o' which produces a
	# not relocatable dll.

	# first generate the base file (it contains relocation information)
	gcc -Wl,--base-file,driver.base \
		-Wl,--entry,_DriverEntry \
		-nostartfiles -nostdlib \
		-o junk.tmp \
		driver.o \
		-lntoskrnl -lhal
	# remove the PE executable generated (not relocatable executable image)
	rm junk.tmp
	# second generate the export file (contains the relocation info, the export symbol and the dll name)
	dlltool --dllname my_hello_driver.sys \
		--base-file driver.base --output-exp driver.exp
	# remove the base file
	rm driver.base
	# and now we can produce the driver...
	gcc -v -Wl,--trace -mdll \
		-Wl,--entry,_DriverEntry \
		-Wl,--subsystem,native \
		-Wl,--image-base,0x10000 \
		-Wl,--file-alignment,0x1000 \
		-Wl,--section-alignment,0x1000 \
		-Wl,--stack,0x40000 \
		-Wl,driver.exp \
		-nostdlib -nostartfiles \
		-o my_hello_driver.sys \
		driver.o \
		-lntoskrnl -lhal

clean:
	rm -f *sys *o *base *tmp *pe *exp *def *dll.a


dll_import:
	# THIS DOES NOT WORK BECAUSE WE NEED STDCALL SYMBOLS...
	#dlltool -z ntoskrnl.def --export-all-symbol "C:/WINDOWS/system32/ntoskrnl.exe"
	echo NAME ntoskrnl > ntoskrnl.def
	echo EXPORTS >> ntoskrnl.def
	pe_reader "C:/WINDOWS/system32/ntoskrnl.exe" | grep "ExportSymbol" | perl -pe "s/^ExportSymbol: (.*) \\(0x.*$$/\\1/g" >> ntoskrnl.def
	dlltool -d ntoskrnl.def -l ntoskrnl.dll.a
