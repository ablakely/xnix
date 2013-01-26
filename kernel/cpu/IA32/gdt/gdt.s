global gdt_flush
extern gp				; from gdt.c

gdt_flush:
	mov	eax, [esp+4]		; Get the pointer to the GDT
	lgdt	[eax]			; Load the GDT pointer

	mov	ax, 0x10		; 0x10 is the offset to our data segment.
	mov	ds, ax			; Load all data segment selectors
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax

	jmp	0x08:.flush		; 0x08 is the offset to our code sement.

.flush:
	ret

[GLOBAL tss_flush]
tss_flush:
        mov     ax, 0x2B        ; load the index of our TSS structure

        ltr     ax              ; load into the task state register
        ret


