ENTRY(start)
phys = 0x001000000;

SECTIONS
{
	.text phys : AT(phys)
	{
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}

	.data : AT(phys + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	end = .;
}
