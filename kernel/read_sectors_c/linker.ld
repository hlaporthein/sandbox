ENTRY(debut);
SECTIONS
{
	. = 0x7C00;
	.text : AT(0x7C00) {
		*(.boot);
	}
	. = 0x7DFC;
	.rdata : {
		LONG(0xaa550000);
		*(.text);
		*(.rdata)
	}
}
