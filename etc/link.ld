ENTRY(start)
SECTIONS
{
	.text 0x10000000 :
	{
		code = .; __code = .;
		*(.text)
		. = ALIGN(4096);
	}

	.data :
	{
		data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	end = .; _end = .; __end = .;
}
