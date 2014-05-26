ENTRY(debut);
SECTIONS
{
    . = 0x7C00;
    .text : AT(0x7C00)
    {
        _text = .;
        *(.text);
        *(.rdata)
        _text_end = .;
    }
    . = 0x7DFC;
    .rdata :
    {
    	LONG(0xaa550000);
    }
}
