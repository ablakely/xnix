ENTRY(start)
SECTIONS
{

    . = 0x00100000;
    .text :{
        code = .;
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
         data = .;
         *(.data)
    }

    .bss : {
        bss = .;
        *(COMMON)
        *(.bss)
    }

    end = .;
}
