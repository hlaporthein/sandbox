ENTRY(debut);
SECTIONS
{
	. = 0x7C00;
	.text : AT(0x7C00) {
		*(.boot);
		*(.bootd);
	}
	. = 0x7DFC;
	.rdata : {
		LONG(0xaa550000);
		*(.text);
		*(.rdata)
		*(.data)
	}
}
