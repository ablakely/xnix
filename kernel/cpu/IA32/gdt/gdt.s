[BITS 32]
global gdt_flush
extern gp				; from gdt.c

gdt_flush:
	lgdt	[gp]			; Load the GDT with the gp pointer
	mov ax, 0x10			; 0x10 is the offset in the GDT to our data segment
	mov ds, ax
	mov es, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2			; far jump; 0x08 is the offset to the code segment

flush2:
	ret				; return


